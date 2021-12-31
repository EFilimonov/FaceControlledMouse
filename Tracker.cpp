#include "pch.h"
#include "Tracker.h"
/*

HHOOK MouseHook;
bool isGlobalMouseMove = false;


LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;
	if (pMouseStruct != NULL) {
		if (wParam == WM_MOUSEMOVE)
		{
			isGlobalMouseMove = true;
		}
		else isGlobalMouseMove = false;
	}
	return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}


void PointsTracker::OnSetMousehook()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, NULL);
	//AfxMessageBox(L"");
}


void PointsTracker::OnUnhookMouse()
{
	UnhookWindowsHookEx(MouseHook);
	OnSetMousehook();
}

void PointsTracker::UnhookMouse()
{
	UnhookWindowsHookEx(MouseHook);
}*/

int PointsTracker::getAngleABC(cv::Point2d a, cv::Point2d b, cv::Point2d c)
{
	cv::Point2d ab = { b.x - a.x, b.y - a.y };
	cv::Point2d cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (int)floor(alpha * 180. / PI + 0.5);
}

cv::Point2f PointsTracker::rotate2d(const cv::Point2f& inPoint, const double& angRad)
{
	cv::Point2f outPoint;
	//CW rotation
	outPoint.x = std::cos(angRad) * inPoint.x - std::sin(angRad) * inPoint.y;
	outPoint.y = std::sin(angRad) * inPoint.x + std::cos(angRad) * inPoint.y;
	return outPoint;
}

cv::Point2f PointsTracker::rotatePoint(const cv::Point2f& inPoint, const cv::Point2f& center, const double& angRad)
{
	return rotate2d(inPoint - center, angRad) + center;
}


void PointsTracker::geoSmileCalculation(cv::Mat& im, std::vector<cv::Point2f>& landmarks)
{

	if (landmarks.size() == 68)
	{

		smilingAngle = -getAngleABC(landmarks[48], landmarks[30], landmarks[54]);

		// smoothing
		smilingAngle = ewmaSmileFilterObj.filter(smilingAngle);

		// pose
		noseNoseLenth = (int)(landmarks[30].y - landmarks[27].y);
		eyeEyeLenth = (int)(landmarks[45].x - landmarks[36].x);
		rotationHeadRatio = (float)noseNoseLenth / (float)eyeEyeLenth;

		//polinom of correction ratio
		smilingAnCalcCorr = 13.054 * rotationHeadRatio * rotationHeadRatio - 7.8506 * rotationHeadRatio + 1.7604;

		smilingAnCalcCorr = ewmasmilingAnCalcCorrObj.filter(smilingAnCalcCorr);

		correctedAng = (smilingTriggerAngle - smilingAngle / smilingAnCalcCorr) / 2;

		if (smilingAngle / smilingAnCalcCorr > smilingTriggerAngle) smileGeoDetectedFlag = true;
		else smileGeoDetectedFlag = false;

		if (showVideoFlag)
		{
			if (!smileGeoDetectedFlag)
			{
				corrAB = rotatePoint(landmarks[48], landmarks[30], correctedAng * (PI / 180.));
				corrBC = rotatePoint(landmarks[54], landmarks[30], -correctedAng * (PI / 180.));
				cv::line(im, landmarks[30], corrAB, CORRECTIONCOLOR, 1);
				cv::line(im, landmarks[30], corrBC, CORRECTIONCOLOR, 1);
			}
			else
			{
				corrAB = rotatePoint(landmarks[48], landmarks[30], correctedAng * (PI / 180.));
				corrBC = rotatePoint(landmarks[54], landmarks[30], -correctedAng * (PI / 180.));
				cv::line(im, landmarks[30], corrAB, CORRECTIONALERTCOLOR, 3);
				cv::line(im, landmarks[30], corrBC, CORRECTIONALERTCOLOR, 3);
			}
			//draw smiling parameters
			arrowedLine(im, landmarks[30], landmarks[48], POINTCOLOR, 1);
			arrowedLine(im, landmarks[30], landmarks[54], POINTCOLOR, 1);
		}
	}

	//circle(im, landmarks[30], 3, POINTCOLOR, FILLED);//Nose tip
	//circle(im, landmarks[48], 4, POINTCOLOR, FILLED);//Left corner of outer lips
	//circle(im, landmarks[54], 4, POINTCOLOR, FILLED);//Right corner of outer lips
	//circle(im, landmarks[62], 4, COLOR, FILLED);//Center of upper lip
	//circle(im, landmarks[66], 4, COLOR, FILLED);//Center of lower lip
	//circle(im, landmarks[8], 4, POINTCOLOR, FILLED);//Jaw bottom
	//circle(im, landmarks[27], 3, POINTCOLOR, FILLED);//Upper nose
	//circle(im, landmarks[57], 4, POINTCOLOR, FILLED);//Lower lips
	//circle(im, landmarks[36], 3, POINTCOLOR, FILLED);//Left eye 
	//circle(im, landmarks[45], 3, POINTCOLOR, FILLED);//Right eye


}



// Get horizontal and vertical screen sizes in pixel
void PointsTracker::GetScreenRatio()
{

	CRect MyRect;

	CWnd::GetDesktopWindow()->GetWindowRect(MyRect);

	horLimit = (int)MyRect.Width();
	vertLimit = (int)MyRect.Height();

	captureHorizontal = (float)cap.get(cv::CAP_PROP_FRAME_WIDTH);
	captureVertical = (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

}


void PointsTracker::initiateTracker()
{
	GetScreenRatio();

	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	cursorPos.x = 0;
	cursorPos.y = 0;


	termcrit.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
	termcrit.max_iter = 6;
	termcrit.epsilon = 0.03;

	cropToTrack.x = (int)captureHorizontal / 3;
	cropToTrack.y = (int)captureVertical / 3;

	cropToTrack.width = (int)captureHorizontal / 3;
	cropToTrack.height = (int)captureVertical / 3;

	cropEqualizer = cropToTrack;


	// minimum allowed movement to exclude trembling
	ultraSmallDelta = captureHorizontal / ULTRASMALL_DELTA_LIMIT;

	ewmaSmileFilterObj.alpha = ewmaSmileAlpha;
	ewmasmilingAnCalcCorrObj.alpha = ewmaSmileAlpha;
	facialFilterX.alpha = ewmaAlpha;
	facialFilterY.alpha = ewmaAlpha;

	maxCornersCount = (int) minCornersCount * 1.7;

	//OnSetMousehook();


}

void PointsTracker::preTrackingActions()
{
	// patch for detector threading
	oldfaces = faces;

		frontalFaceDetector.detectMultiScale(frame_gray, faces, 1.1, minFaceNeighbors, cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_CANNY_PRUNING, cv::Size(65, 65));
		// if face detected
		if (!faces.empty())
		{
			tempinptext = "";
			mMouseDlg->changePie(mMouseDlg->NEUTRAL);
			faceDetectedFlag = true;
			faceInitiatedFlag = true;
			// calculate ROI by face

			cropToTrack.x = faces[0].x + (int)faces[0].width * 0.2;
			cropToTrack.y = faces[0].y;

			// face too close or far
			cropToTrack.width = faces[0].width * 0.6;
			cropToTrack.height = faces[0].height * 0.65;


			if (needEqualize)
			{
				// calculate equalizer area by face
				cropEqualizer.x = faces[0].x - (int)faces[0].width * 0.2;
				cropEqualizer.y = faces[0].y - (int)faces[0].height * 0.2;

				if ((int)faces[0].width * 1.5 < frame_gray.size().width) cropEqualizer.width = (int)faces[0].width * 1.4;
				if ((int)faces[0].height * 1.5 < frame_gray.size().height) cropEqualizer.height = (int)faces[0].height * 1.4;
			}

		}

		else // if face not detected
		{
			faceDetectedFlag = false;
			tempinptext = facetext;
			mMouseDlg->changePie(mMouseDlg->LOSTFACE);

			// if face not found move rectancle with tracking points
			if (dMouse.x < cropToTrack.width && dMouse.y < cropToTrack.height)
			{
				cropToTrack.x += dMouse.x;
				cropToTrack.y += dMouse.y;
			}

			if (needEqualize)
			{
				if (cropEqualizer.width < frame_gray.size().width)cropEqualizer.width++;
				if (cropEqualizer.x > 0)cropEqualizer.x--;
				if (cropEqualizer.height < frame_gray.size().height)cropEqualizer.height++;
				if (cropEqualizer.y > 0)cropEqualizer.y--;
			}

		}



		// check crop borders to not exceed frame size
		cropToTrack = calculateCrop(cropToTrack, frame_gray.size());

		if (needEqualize)
		{
			cropEqualizer = calculateCrop(cropEqualizer, frame_gray.size());
			equalizerROI = frame_gray(cropEqualizer);
			equalizeHist(equalizerROI, equalizerROI);
			frame_gray.copyTo(equalizerROI);
		}

		boxWidth = (float)cropToTrack.width / boxRatio;
		boxHeight = (float)cropToTrack.height / boxRatio;

}


bool PointsTracker::Tracking()
{

{ 

		if (!openFlag)
		{
			cv::destroyAllWindows();
			return false;
		}

		cap.read(frame);

		if (frame.empty())
		{
			MessageBox(NULL, _T("No captured frame"), NULL, NULL);
			return false;
		}

//	inptext = "elapsedSec= " + std::to_string(mMouseDlg->elapsedSeconds) + "secToOneClick= " + std::to_string(mMouseDlg->secToOneClickDuration) + "secToDoubleClick = " + std::to_string(mMouseDlg->secToDoubleClickDuration) + " allDuration = " + std::to_string(mMouseDlg->secDuration);
	// rotatePie(elapsedSeconds, false, false, secSmile, secToOneClickDuration, secToDoubleClickDuration, secDuration);
	smileAIDetectedFlag = false;
	smileGeoDetectedFlag = false;

	//inptext = "turnOffClick = " + std::to_string(turnOffClick) + "buttonStop = " + std::to_string(buttonStop) + "mouseHookPause = " + std::to_string(mouseHookPause) + "sec = " + std::to_string(pauseTime - moveLockTimer.elapsedSeconds());


	try {

	//if (flipCameraFlag)cv::flip(frame, frame, 1);

	// Convert input to greyscale 
	cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

	if (showVideoFlag)
	{

		cv::putText(frame, tempinptext, cv::Point(40, 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(CORRECTIONALERTCOLOR), 2, cv::LINE_AA);

		cv::rectangle(frame, cropToTrack, POINTCOLOR, 1, 8, 0);	
	}

	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	// start tracking

		if (needToTrackerInit)
		{
			cv::Mat mask(frame_gray.size(), CV_8UC1, cv::Scalar(0));
			mask(cropToTrack).setTo(cv::Scalar(255));

			// minimum dist between tracking points 
			minDist = cropToTrack.width / minDistRatio;

			pointsT.clear();

			goodFeaturesToTrack(frame_gray, pointsT, maxCornersCount, QUALITY_LEVEL, minDist, mask, 3, 3, 0, 0.04);
			if (pointsT.size() < minCornersCount)
			{
				needToTrackerInit = true;

				if (showVideoFlag) imshow("view", frame);
				int key2 = cv::waitKey(20);

				return true;
			}

			cornerSubPix(frame_gray, pointsT, cvSize(10, 10), cv::Size(-1, -1), termcrit);

			activeTrackers = pointsT.size();

			pointsMosse.clear();

			pointsMosse.resize(activeTrackers);

			skipframe = true;

			trackingRectangle.x = cropToTrack.x - cropToTrack.width * 0.5;
			trackingRectangle.y = cropToTrack.y - cropToTrack.height * 0.5;

			trackingRectangle.width = (int)cropToTrack.width * 2;
			trackingRectangle.height = (int)cropToTrack.height * 2;

			trackingRectangle = calculateCrop(trackingRectangle, frame_gray.size());


			boxWidth = (float)cropToTrack.width / boxRatio;
			boxHeight = (float)cropToTrack.height / boxRatio;

			for (int i = 0; i < activeTrackers; i++)
				if (!pointsT.empty())
				initFacialTracker(i, frame_gray(trackingRectangle));

		}

		else if (needToAddPoints)
		{
			addNewPoints();
		}

		calculateMosseTrackers(frame, frame_gray(trackingRectangle));

		if (activeTrackers < minCornersCount)
		{
			pointsMosse.clear();
			needToTrackerInit = true;
		}
		else
		{
			needToTrackerInit = false;
		}


		/// end tracking
		/////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////
		/// start smile detecting
			/////////////////////////////////////////////////////////////
			// AI smile detecting

		if (detectSmileAIFlag)
		{

			cv::Rect r;
			if (faceDetectedFlag) r = faces[0];
			else
			{
				if (needEqualize) r = cropEqualizer;
				else r = cropToTrack;
			}

			const int half_height = cvRound((float)r.height / 2);
			r.y = r.y + half_height;
			r.height = half_height - 1;

			//In half-face detect smile
			smileDetector.detectMultiScale(frame_gray(r), smiles, 1.1, minNeighborsSmileDetector, cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

			if (smiles.size() > 0)
			{
				for (size_t j = 0; j < smiles.size(); j++)
				{
					smileAIDetectedFlag = true;
					if (showVideoFlag)
					{
						smileAIrect = cv::Rect(r.x + smiles[j].x, r.y + smiles[j].y, smiles[j].width, smiles[j].height);
						cv::rectangle(frame, smileAIrect, CORRECTIONALERTCOLOR, 2, 8, 0);
					}

				}
			}
			else smileAIDetectedFlag = false;

		}

		/////////////////////////////////////////////////////////////
		// Geometrical smile detecting
		if (detectSmileGeoFlag && faceDetectedFlag)
		{		
			// patch for detector threading
			if (faces.empty()) faces = oldfaces;
			// find facemarks
			facemarkDetectedFlag = facemark->fit(frame_gray, faces, landmarks);
			// if facemarks detected
			if (facemarkDetectedFlag)geoSmileCalculation(frame, landmarks[0]);
		}

	}// end try
	catch (const std::exception& e)
	{

		smileAIDetectedFlag = false;
		smileGeoDetectedFlag = false;

		mMouseDlg->smileMouseLocked = true;
		mMouseDlg->mouseTimer.stop();

		mMouseDlg->dwellMouseLocked = true;
		mMouseDlg->dwellTimer.stop();

		needToTrackerInit = true;
		firstFaceCropFlag = true;

		std::string str1 = e.what();

		inptext = str1;
	
		//CString str2(str1.c_str());
		//MessageBox(NULL, _T("Tracking exception") + str2, NULL, NULL);
	}

	if (showVideoFlag)cv::putText(frame, inptext, cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

	if (showVideoFlag) imshow("view", frame);
	int key2 = cv::waitKey(20);


	if (!mouseHookPause && !buttonStop)
	{
		if (detectDwellFlag && faceInitiatedFlag)
		{
			//disable moving if face changed
			if (mds < cropToTrack.width) mMouseDlg->dwellDetecting(mds);
		}
		else
		{
			if (isQuickClick) mMouseDlg->quickMouseDlg(smileAIDetectedFlag || smileGeoDetectedFlag);
			else mMouseDlg->timerMouseDlg(smileAIDetectedFlag || smileGeoDetectedFlag);
		}
	}


//	inptext = inptext + "elapsedSeconds = " + std::to_string(mMouseDlg->mouseTimer.elapsedSeconds()) + ", smileMouseLocked = " + std::to_string(mMouseDlg->smileMouseLocked) + ", secQuickClick = " + std::to_string(mMouseDlg->secQuickClick);


	//OnUnhookMouse();
	}
	return true;
}


bool PointsTracker::initFacialTracker(int i, cv::Mat& _frame_gray)
{

	pointsT[i].x -= trackingRectangle.x;
	pointsT[i].y -= trackingRectangle.y;
	pointsMosse[i].facialTracker = cv::legacy::TrackerMOSSE::create();

	pointsMosse[i].facialRectangle.width = boxWidth;
	pointsMosse[i].facialRectangle.height = boxHeight;

	pointsMosse[i].facialRectangle.x = pointsT[i].x - boxWidth / 2;
	pointsMosse[i].facialRectangle.y = pointsT[i].y - boxHeight / 2;

	//pointsMosse[i].facialFilterX.alpha = ewmaAlpha;
	//pointsMosse[i].facialFilterX.alpha = ewmaAlpha;

	pointsMosse[i].needInit = false;

	pointsMosse[i].pointNew = pointsT[i];
	pointsMosse[i].pointOld = pointsT[i];

	return pointsMosse[i].facialTracker->init(_frame_gray, pointsMosse[i].facialRectangle);

}


void PointsTracker::calculateMosseTrackers(cv::Mat& _frame, cv::Mat& _frame_gray)
{
	int i = 0;
	activeTrackers = 0;

	dxTrackPointSum = 0.0f;
	dyTrackPointSum = 0.0f;

	cv::Scalar pColor = CORRECTIONCOLOR;

	for (i = 0; i < pointsMosse.size(); i++)
	{
		bool found = pointsMosse[i].facialTracker->update(_frame_gray, pointsMosse[i].facialRectangle);


		pointsMosse[i].pointNew.x = pointsMosse[i].facialRectangle.x;
		pointsMosse[i].pointNew.y = pointsMosse[i].facialRectangle.y;

		cv::Point _newt(pointsMosse[i].pointNew.x + boxWidth / 2, pointsMosse[i].pointNew.y + boxHeight / 2);
		_newt.x += trackingRectangle.x;
		_newt.y += trackingRectangle.y;

		if (found)
		{

			if (_newt.x >= cropToTrack.x &&
				_newt.x <= cropToTrack.x + cropToTrack.width &&
				_newt.y >= cropToTrack.y &&
				_newt.y <= cropToTrack.y + cropToTrack.height)
				{
					pColor = CORRECTIONCOLOR;


						if (pointsMosse[i].needInit) // skip movement for new
						{
							pointsMosse[i].needInit = false;
							pointsMosse[i].pointOld = pointsMosse[i].pointNew;
							continue;
						}

						pointsMosse[i].pointDelta = pointsMosse[i].pointNew - pointsMosse[i].pointOld;

						if (abs(pointsMosse[i].pointDelta.x) > boxWidth || abs(pointsMosse[i].pointDelta.y) > boxWidth)
						{
							pointsMosse[i].pointOld = pointsMosse[i].pointNew;
							continue;
						}

						else
						{

							dxTrackPointSum += pointsMosse[i].pointDelta.x;
							dyTrackPointSum += pointsMosse[i].pointDelta.y;
							if (showVideoFlag)
							{
								if (pointsMosse[i].pointDelta.x > -boxWidth / 3 && pointsMosse[i].pointDelta.y > -boxHeight / 3 )
								cv::arrowedLine(_frame, _newt, cv::Point(_newt.x + pointsMosse[i].pointDelta.x * 10, _newt.y + pointsMosse[i].pointDelta.y * 10), CORRECTIONCOLOR, 1, 8, 0);
							}
						
							activeTrackers++;
						}
						pointsMosse[i].pointOld = pointsMosse[i].pointNew;
				}

			else // delete if out of area
			{
				pointsMosse.erase(pointsMosse.begin() + i);
				i--;
				continue;
			}

		}
		else // delete if not found
		{
			pointsMosse.erase(pointsMosse.begin() + i);
			i--;
			continue;
		}
		if (showVideoFlag)cv::circle(_frame, _newt, 3, pColor, cv::FILLED);
		cv::rectangle(frame, trackingRectangle, CORRECTIONCOLOR, 2, 8, 0);	
	}

	if(!skipframe && !mouseHookPause && (dxTrackPointSum || dyTrackPointSum))
	{ 
		
		
		dMouse.x = (float)dxTrackPointSum / activeTrackers;
		dMouse.y = (float)dyTrackPointSum / activeTrackers;
		if (flipCameraFlag) dMouse.x = -dMouse.x;
		calclMousePos(dMouse, cropToTrack.width);
		mds = sqrt(dMouse.x * dMouse.x + dMouse.y * dMouse.y);

	}

	if (activeTrackers < minCornersCount - 2) needToTrackerInit = true;

	if (activeTrackers < minCornersCount + 3) needToAddPoints = true;
		else needToAddPoints = false;

	skipframe = false;

}



void PointsTracker::addNewPoints()
{
	cv::Mat mask(frame_gray.size(), CV_8UC1, cv::Scalar(0));
	mask(cropToTrack).setTo(cv::Scalar(255));

	// minimum dist between tracking points 
	minDist = cropToTrack.width / minDistRatio;

	//float minPointDist = cropToTrack.width / 10;

	pointsT.clear();

	goodFeaturesToTrack(frame_gray, pointsT, minCornersCount, 0.01, minDist, mask, 3, 3, 0, 0.04);
	if (!pointsT.empty()) 	cornerSubPix(frame_gray, pointsT, cvSize(10, 10), cv::Size(-1, -1), termcrit);
	else return;

	float ptdx = boxWidth / 2 + trackingRectangle.x;
	float ptdy = boxHeight / 2 + trackingRectangle.y;

	boxWidth = (float)cropToTrack.width / boxRatio;
	boxHeight = (float)cropToTrack.height / boxRatio;

	cv::Point2f candidate;

	bool isCandidateOK = false;

	int countToAdd = 0;

	float minPtDist = minDist / 2;

	for (cv::Point2f ptCorner : pointsT)
	{

		if (ptCorner.x <= cropToTrack.x ||
			ptCorner.x >= cropToTrack.x + cropToTrack.width ||
			ptCorner.y <= cropToTrack.y ||
			ptCorner.y >= cropToTrack.y + cropToTrack.height)
			continue;

		candidate = ptCorner;

		for (PointMosse ptMosse : pointsMosse)
		{
			if (ptMosse.facialRectangle.x + ptdx - ptCorner.x > minPtDist || ptMosse.facialRectangle.y + ptdy - ptCorner.y > minPtDist)
			{
				isCandidateOK = true;
			}
			else
			{
				isCandidateOK = false;
				break;
			}
		}

		if (isCandidateOK)
		{
			countToAdd++;
			addFacialTracker(candidate, frame_gray(trackingRectangle));
			if (showVideoFlag) circle(frame, candidate, 3, POINTCOLOR, cv::FILLED);
			if (countToAdd > 2) return;
			isCandidateOK = false;
		}
	}

}


void PointsTracker::addFacialTracker(cv::Point2f _pt, cv::Mat& _frame_gray)
{

	_pt.x -= trackingRectangle.x;
	_pt.y -= trackingRectangle.y;

	PointMosse _pointMosse;
	_pointMosse.facialTracker = cv::legacy::TrackerMOSSE::create();


	_pointMosse.facialRectangle.width = boxWidth;
	_pointMosse.facialRectangle.height = boxHeight;

	_pointMosse.facialRectangle.x = _pt.x - boxWidth / 2;
	_pointMosse.facialRectangle.y = _pt.y - boxHeight / 2;
	_pointMosse.facialTracker->init(_frame_gray, _pointMosse.facialRectangle);

	_pointMosse.pointNew = _pt;
	_pointMosse.pointOld = _pt;

	_pointMosse.needInit = true;

	pointsMosse.push_back(_pointMosse);

	//pointsMosse[i].facialFilterX.alpha = ewmaAlpha;
	//pointsMosse[i].facialFilterX.alpha = ewmaAlpha;


}


void PointsTracker::calclMousePos(cv::Point2f deltaPoint, int faceH)
{


		//mds = sqrt(deltaPoint.x * deltaPoint.x + deltaPoint.y * deltaPoint.y);
		mv = mds / mdt;

		velocityScale = velocityK * mv + 0.7;

		horScale = velocityScale * (float)horLimit / (float)faceH;
		verScale = velocityScale * (float)vertLimit / (float)faceH;

		correctedDX = (LONG)(deltaPoint.x * horScale * horSensitivity);
		correctedDY = (LONG)(deltaPoint.y * verScale * verSensitivity);

		facialFilterX.alpha = ewmaAlpha;
		facialFilterY.alpha = ewmaAlpha;

		correctedDX = facialFilterX.filter(correctedDX);
		correctedDY = facialFilterY.filter(correctedDY);

		GetCursorPos(&cursorPos);

		mouseX = cursorPos.x + correctedDX;
		mouseY = cursorPos.y + correctedDY;

		if (mouseX > horLimit) mouseX = horLimit;
		if (mouseY > vertLimit) mouseY = vertLimit;
		if (mouseX < 1) mouseX = 0;
		if (mouseY < 1) mouseY = 0;

		if (mMouseDlg->mainSelfDragflag && mMouseDlg->mouseClick == mMouseDlg->DRAG)::SendMessage(hWnd, UWM_CUSTOMDRAGMAIN, NULL, 0);
		if (mMouseDlg->optSelfDragflag && mMouseDlg->mouseClick == mMouseDlg->DRAG)::SendMessage(hWnd, UWM_CUSTOMDRAGOPTION, NULL, 0);

}


cv::Rect PointsTracker::calculateCrop(cv::Rect _crop, cv::Size _frame)
{

	if ((_crop.x + _crop.width) > _frame.width)
		_crop.x = _frame.width - _crop.width - 2;

	if ((_crop.y + _crop.height) > _frame.height)
		_crop.y = _frame.height - _crop.height - 2;

	if (_crop.x < 0)_crop.x = 2;
	if (_crop.y < 0)_crop.y = 2;

	return _crop;

}



