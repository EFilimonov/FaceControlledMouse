#include "pch.h"
#include "Tracker.h"


HHOOK MouseHook;
bool isGlobalMouseMove = false;
/*
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
	if (wParam == WM_MOUSEMOVE) isGlobalMouseMove = true;
	else isGlobalMouseMove = false;

//	if (wParam == WM_LBUTTONDOWN || WM_LBUTTONUP || WM_LBUTTONDBLCLK || WM_RBUTTONDOWN || WM_RBUTTONUP)isGlobalMouseMove = false;


	return TRUE;

}
*/




LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if (pMouseStruct != NULL){
		if(wParam ==  WM_MOUSEMOVE)
		{
			isGlobalMouseMove = true;
		}
		else isGlobalMouseMove = false;;
	}
	return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}





void Tracker::OnSetMousehook()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, NULL);

}


void Tracker::OnUnhookMouse()
{
	UnhookWindowsHookEx(MouseHook);

}


int Tracker::getAngleABC(cv::Point2d a, cv::Point2d b, cv::Point2d c)
{
	cv::Point2d ab = { b.x - a.x, b.y - a.y };
	cv::Point2d cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (int)floor(alpha * 180. / PI + 0.5);
}

cv::Point2f Tracker::rotate2d(const cv::Point2f& inPoint, const double& angRad)
{
	cv::Point2f outPoint;
	//CW rotation
	outPoint.x = std::cos(angRad) * inPoint.x - std::sin(angRad) * inPoint.y;
	outPoint.y = std::sin(angRad) * inPoint.x + std::cos(angRad) * inPoint.y;
	return outPoint;
}

cv::Point2f Tracker::rotatePoint(const cv::Point2f& inPoint, const cv::Point2f& center, const double& angRad)
{
	return rotate2d(inPoint - center, angRad) + center;
}



void Tracker::geoSmileCalculation(cv::Mat& im, std::vector<cv::Point2f>& landmarks)
{

	if (landmarks.size() == 68)
	{

		smilingAngle = -getAngleABC(landmarks[48], landmarks[30], landmarks[54]);

		// smoothing
		smilingAngle = ewmaSmileAnglefilter->filter(smilingAngle);

		// pose
		noseNoseLenth = (int)(landmarks[30].y - landmarks[27].y);
		eyeEyeLenth = (int)(landmarks[45].x - landmarks[36].x);
		rotationHeadRatio = (float)noseNoseLenth / (float)eyeEyeLenth;

		//polinom of correction ratio
		smilingAnCalcCorr = 13.054 * rotationHeadRatio * rotationHeadRatio - 7.8506 * rotationHeadRatio + 1.7604;

		smilingAnCalcCorr = ewmasmilingAnCalcCorr->filter(smilingAnCalcCorr);

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
void Tracker::GetScreenRatio()
{

	CRect MyRect;

	CWnd::GetDesktopWindow()->GetWindowRect(MyRect);

	horLimit = (int)MyRect.Width();
	vertLimit = (int)MyRect.Height();

	captureHorizontal = (float)cap.get(cv::CAP_PROP_FRAME_WIDTH);
	captureVertical = (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

}


void Tracker::Tracking()
{

	GetScreenRatio();

	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	cursorPos.x = 0;
	cursorPos.y = 0;
	oldCursorPos = cursorPos;

	CvTermCriteria termcrit;
	termcrit.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
	termcrit.max_iter = 6;
	termcrit.epsilon = 0.03;

	std::vector<cv::Point2f> pointsT[2];
	std::vector<cv::Point2f> pointsTDelta[1];

	float captureHorizontal = (float)cap.get(cv::CAP_PROP_FRAME_WIDTH);
	float captureVertical = (float)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cropToTrack.x = (int)captureHorizontal / 3;
	cropToTrack.y = (int)captureVertical / 3;

	cropToTrack.width = (int)captureHorizontal / 3;
	cropToTrack.height = (int)captureVertical / 3;

	cropEqualizer = cropToTrack;

	bool firstFaceCropFlag = true;

	cv::Mat frame, frame_gray, prevGray, faceROI, equalizerROI;
	// Vector of returned faces
	std::vector<cv::Rect> faces;

	std::vector<std::vector<cv::Point2f> > landmarks;


	// minimum allowed movement to exclude trembling
	smallDelta = captureHorizontal / SMALL_DELTA_LIMIT;
	ultraSmallDelta = captureHorizontal / ULTRASMALL_DELTA_LIMIT;


	/////////////////////////////////////

	Ewma ewmaXfilterObj(ewmaAlpha);
	Ewma ewmaYfilterObj(ewmaAlpha);

	ewmaXfilter = &ewmaXfilterObj;
	ewmaYfilter = &ewmaYfilterObj;

	Ewma ewmaSmileFilterObj(ewmaSmileAlpha);
	Ewma ewmasmilingAnCalcCorrObj(ewmaSmileAlpha);
	ewmaSmileAnglefilter = &ewmaSmileFilterObj;
	ewmasmilingAnCalcCorr = &ewmasmilingAnCalcCorrObj;

	////////////////////////////////////

	secToOneClickDuration = secSmile + secOneClick;
	secToDoubleClickDuration = secSmile + secOneClick + secDoubleClick;
	secToOneClickCancelDuration = secSmile + secOneClick + secCancel;
	secToDoubleClickCancelDuration = secSmile + secOneClick + secDoubleClick + secCancel;

	for (;;)
	{
		
		mMouseDlg->moveMouseDlg();


		// set hook to detect mouse move
		if(!isGlobalMouseMove) OnSetMousehook();
		
		else// if move
		{	
			// set timer
			moveLockTimer.start();
			// dont move cursor by tracker
			mouseHookPause = true;
			// dont click
			turnOffClick = true;
			// reset flag
			isGlobalMouseMove = false;
		}


		if (moveLockTimer.elapsedSeconds() > 0)
		{
			
			mMouseDlg->changePie(mMouseDlg->PAUSE);
			OnUnhookMouse();			
		}	

		if (moveLockTimer.elapsedSeconds() > pauseTime)
		{
			mouseHookPause = false;
			mMouseDlg->changePie(mMouseDlg->NEUTRAL);
			moveLockTimer.stop();
			if (!buttonStop) turnOffClick = false;
		}

		// possibility to start even when tracker stopped
		if (buttonStop)
		{
			::SendMessage(hWnd, UWM_CUSTOMSTARTTRACK, NULL, 0);
			GetCursorPos(&cursorPos);
			if (cursorPos.y > buttonStartPosition.top && cursorPos.y < buttonStartPosition.bottom
				&& cursorPos.x > buttonStartPosition.left && cursorPos.x < buttonStartPosition.right)
			{

				dwellDetecting(0);
			}
			else
			{
				dwellMouseLocked = true;
				mMouseDlg->changePie(mMouseDlg->NEUTRAL);
				if (mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_HIDE);
			}
		}


		//inptext = "turnOffClick = " + std::to_string(turnOffClick) + "buttonStop = " + std::to_string(buttonStop) + "mouseHookPause = " + std::to_string(mouseHookPause) + "sec = " + std::to_string(pauseTime - moveLockTimer.elapsedSeconds());
	
		if (!openFlag)
		{
			cv::destroyAllWindows();
			break;
		}

		cap.read(frame);

		if (frame.empty())
		{
			MessageBox(NULL, _T("No captured frame"), NULL, NULL);
			break;
		}

		//inptext = "num0 = " + std::to_string(defaultThreadsNum) + "num1 = " + std::to_string(cv::getNumThreads());

		//if (flipCameraFlag)cv::flip(frame, frame, 1);

		// Convert input to greyscale 
		cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

		// Detecting faces
		frontalFaceDetector.detectMultiScale(frame_gray, faces, 1.1, minFaceNeighbors, cv::CASCADE_FIND_BIGGEST_OBJECT | cv::CASCADE_DO_CANNY_PRUNING, cv::Size(65, 65));

		try {
			// if face detected
			if (!faces.empty())
			{
				faceDetectedFlag = true;
				faceInitiatedFlag = true;
				// calculate ROI by face

				cropToTrack.x = faces[0].x + (int)faces[0].width * 0.2;
				cropToTrack.y = faces[0].y;

				// face too close or far
				//if (cropToTrack.width * 1.8 < faces[0].width || cropToTrack.width > faces[0].width * 0.7)
				//{
				cropToTrack.width = faces[0].width * 0.6;
				cropToTrack.height = faces[0].height * 0.65;
				//	needToTrackerInit = true;
				//}

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
				if (showVideoFlag)
				{
					tempinptext = "Face not detected";
					cv::putText(frame, tempinptext, cv::Point(40, 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, cv::Scalar(CORRECTIONALERTCOLOR), 2, cv::LINE_AA);
				}
				// if face not found move rectancle with tracking points
				cropToTrack.x += dxTrackPoint;
				cropToTrack.y += dyTrackPoint;

				if (needEqualize)
				{
					if (cropEqualizer.width < frame_gray.size().width)cropEqualizer.width++;
					if (cropEqualizer.x > 0)cropEqualizer.x--;
					if (cropEqualizer.height < frame_gray.size().height)cropEqualizer.height++;
					if (cropEqualizer.y > 0)cropEqualizer.y--;
				}

			}

			/////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////
			// start tracking

			// check crop borders to not exceed frame size
			cropToTrack = calculateCrop(cropToTrack, frame_gray.size());
			if (showVideoFlag) cv::rectangle(frame, cropToTrack, POINTCOLOR, 1, 8, 0);

			if (needEqualize)
			{
				cropEqualizer = calculateCrop(cropEqualizer, frame_gray.size());
				equalizerROI = frame_gray(cropEqualizer);
				equalizeHist(equalizerROI, equalizerROI);
				frame_gray.copyTo(equalizerROI);
			}


			//imshow("view2", frame_gray);
			//int key2 = cv::waitKey(20);

			//minDist = cropToTrack.width / minDistRatio;
			//inptext = "width = " + std::to_string(cropToTrack.width) + "minDist = " + std::to_string(minDist) + "R = " + std::to_string(minDistRatio);

			if (needToTrackerInit)
			{
				cv::Mat mask(frame_gray.size(), CV_8UC1, cv::Scalar(0));
				mask(cropToTrack).setTo(cv::Scalar(255));

				// minimum dist between tracking points 
				
				//maxCornersCount = (int) (1.5 * minCornersCount + 15);

					pointsT[1].clear();
					pointsT[0].clear();
					pointsTDelta[0].clear();

				goodFeaturesToTrack(frame_gray, pointsT[1], maxCornersCount, QUALITY_LEVEL, minDist, mask, 3, 3, 0, 0.04);
				if (pointsT[1].size() < minCornersCount)
				{
					needToTrackerInit = true;

					if (showVideoFlag) imshow("view", frame);
					int key2 = cv::waitKey(20);
					continue;
				}

				cornerSubPix(frame_gray, pointsT[1], cvSize(10, 10)/*(10, 10)*/, cv::Size(-1, -1), termcrit);

			}
			dxTrackPointSum = 0;
			dyTrackPointSum = 0;
			dxTrackPoint = 0;
			dyTrackPoint = 0;

			if (prevGray.empty()) frame_gray.copyTo(prevGray);
			if (pointsT[0].empty())pointsT[0] = pointsT[1];

			// ////////////////////////////
			pointsTDelta[0].resize(pointsT[1].size());
			// ////////////////////////////

			calcOpticalFlowPyrLK(prevGray, frame_gray, pointsT[0], pointsT[1], status, err, cvSize(31, 31)/*eva(11, 11) 31 31*/, 3, termcrit, 0, 0.001);

			size_t cornerExistNum, k;


			for (cornerExistNum = k = 0; cornerExistNum < pointsT[1].size(); cornerExistNum++)
			{
				if (status[cornerExistNum] &&
					pointsT[1][cornerExistNum].x >= cropToTrack.x &&
					pointsT[1][cornerExistNum].x <= cropToTrack.x + cropToTrack.width &&
					pointsT[1][cornerExistNum].y >= cropToTrack.y &&
					pointsT[1][cornerExistNum].y <= cropToTrack.y + cropToTrack.height)
				{

					dxTrackPoint = pointsT[1][cornerExistNum].x - pointsT[0][cornerExistNum].x;
					dyTrackPoint = pointsT[1][cornerExistNum].y - pointsT[0][cornerExistNum].y;

					// ////////////////////////////
					pointsTDelta[0][cornerExistNum].x = dxTrackPoint;
					pointsTDelta[0][cornerExistNum].y = dyTrackPoint;

					dxTrackPointSum += dxTrackPoint;
					dyTrackPointSum += dyTrackPoint;
					// ////////////////////////////

					if (showVideoFlag)
					{
						cv::drawMarker(frame, pointsT[0][cornerExistNum], CORRECTIONCOLOR, cv::MARKER_SQUARE, 10, 1);
						if ((abs(dxTrackPoint) < ultraSmallDelta) && (abs(dyTrackPoint) < ultraSmallDelta))
						{
							cv::drawMarker(frame, pointsT[1][cornerExistNum], ULTRASMALLMOVECOLOR, cv::MARKER_CROSS, 5, 1);
						}
						else
						{
							cv::line(frame, pointsT[1][cornerExistNum], cv::Point(pointsT[0][cornerExistNum].x + dxTrackPoint * 20, pointsT[0][cornerExistNum].y + dyTrackPoint * 20), SMALLMOVECOLOR, 1, 8, 0);
						}
					}

					pointsT[1][k++] = pointsT[1][cornerExistNum];

				}

			}
			pointsT[1].resize(k);


			pointsTDelta[0].resize(k);


			resultantLenth = dxTrackPointSum * dxTrackPointSum + dyTrackPointSum * dyTrackPointSum;
			addLenth = 0;
			sumVx = 0;
			sumVy = 0;

			for (int i = 0; i < pointsTDelta[0].size(); i++)
			{
				sumVx = dxTrackPointSum + pointsTDelta[0][i].x;
				sumVy = dyTrackPointSum + pointsTDelta[0][i].y;

				// Does delta lengthen the resultant? 
				if ((sumVx * sumVx + sumVy * sumVy) > resultantLenth) addLenth++;
			}

			if (cornerExistNum && (addLenth > accumMoveRatio * (float)cornerExistNum))
			{
				dMouse.x = dxTrackPointSum / (float)cornerExistNum;
				dMouse.y = dyTrackPointSum / (float)cornerExistNum;
				if (flipCameraFlag) dMouse.x = -dMouse.x;
				calclMousePos(dMouse, cropToTrack.width);
				
			}


			if (pointsT[1].size() < minCornersCount)
			{
				//MessageBox(NULL, _T("Face detect exception"), NULL, NULL);
				needToTrackerInit = true;
				pointsT[1].clear();
				pointsT[0].clear();
				pointsTDelta[0].clear();
				//imshow("view", frame);
				//int key2 = cv::waitKey(20); 
				//continue;
			}
			else needToTrackerInit = false;

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
				// find facemarks
				facemarkDetectedFlag = facemark->fit(frame_gray, faces, landmarks);
				// if facemarks detected
				if (facemarkDetectedFlag)geoSmileCalculation(frame, landmarks[0]);

			}
			// end process facemarks
	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
			//end smile detecting

		}// end try
		catch (const std::exception& e)
		{

			smileAIDetectedFlag = false;
			smileGeoDetectedFlag = false;

			smileMouseLocked = true;
			mouseTimer.stop();

			dwellMouseLocked = true;
			dwellTimer.stop();

			needToTrackerInit = true;
			firstFaceCropFlag = true;

			std::string str1 = e.what();

			inptext = str1;
			//tempinptext = str1;
			//cv::putText(frame, tempinptext, cv::Point(60, 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(CORRECTIONALERTCOLOR), 2, cv::LINE_AA);

		//	
		//	CString str2(str1.c_str());
		//	MessageBox(NULL, _T("Tracking exception") + str2, NULL, NULL);
		}

		cv::putText(frame, inptext, cv::Point(20, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

		if (showVideoFlag) imshow("view", frame);
		int key2 = cv::waitKey(20);

		std::swap(pointsT[1], pointsT[0]);
		cv::swap(prevGray, frame_gray);
		GetCursorPos(&cursorPos);
		if (!turnOffClick && !detectDwellFlag)
		{
			if (isQuickClick) quickMouseDlg();
			else timerMouseDlg();
		}
		
		OnUnhookMouse();





	} // end big for

}


void Tracker::calclMousePos(cv::Point2f deltaPoint, int faceH)
{


	if (!mouseHookPause)
	{


	float mdt = 0.04;
	float mds = sqrt(deltaPoint.x * deltaPoint.x + deltaPoint.y * deltaPoint.y);
	float mv = mds / mdt;


		// disable trembling

		if (mds < ultraSmallDelta)
		{
			if (detectDwellFlag && !turnOffClick) 
				dwellDetecting(0);
			return;
		}
		//and disable moving if face changed
		if (mds > cropToTrack.width / 2) return;

		velocityScale = velocityK * mv + 0.7;

		horScale = velocityScale * (float)horLimit / (float)faceH;
		verScale = velocityScale * (float)vertLimit / (float)faceH;

		if (turnOnEWMA)
		{
			deltaPoint.x = ewmaXfilter->filter(deltaPoint.x);
			deltaPoint.y = ewmaYfilter->filter(deltaPoint.y);
		}



		correctedDX = (LONG)(deltaPoint.x * horScale * horSensitivity);
		correctedDY = (LONG)(deltaPoint.y * verScale * verSensitivity);

		//GetCursorPos(&cursorPos);

		mouseX = cursorPos.x + correctedDX;
		mouseY = cursorPos.y + correctedDY;

		/// <summary>
		/// /
		/// </summary>
		/// <param name="deltaPoint"></param>
		/// <param name="faceH"></param>
		if (!mMouseDlg->notDragFlag && mMouseDlg->isNonMainClientArea)::SendMessage(hWnd, UWM_CUSTOMDRAGMAIN, NULL, 0);
		if (!mMouseDlg->notDragFlag && mMouseDlg->isNonOptionsClientArea)::SendMessage(hWnd, UWM_CUSTOMDRAGOPTION, NULL, 0);

		if (mouseX > horLimit) mouseX = horLimit;
		if (mouseY > vertLimit) mouseY = vertLimit;
		if (mouseX < 1) mouseX = 0;
		if (mouseY < 1) mouseY = 0;

		SetCursorPos(mouseX, mouseY);

		if (detectDwellFlag && faceInitiatedFlag && !turnOffClick) dwellDetecting(mds);



	}

}

void Tracker::quickMouseDlg()
{
	if (smileAIDetectedFlag || smileGeoDetectedFlag)

	{
		// break if recent smile
		if (!quickSmileUnlocked) return;

		if (smileMouseLocked)
		{
			mouseTimer.start();
			if (!mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_SHOW);
		}

		smileMouseLocked = false;


		double elapsedSeconds = mouseTimer.elapsedSeconds();

		if (elapsedSeconds < secQuickClick)
		{
			mMouseDlg->rotatePie(elapsedSeconds, false, true, secSmile, secToOneClickDuration, secToDoubleClickDuration, secQuickClick);
			mMouseDlg->RedrawWindow();
		}
		else // if keep smiling
		{
			OnUnhookMouse();
			mMouseDlg->MouseInput();
			mouseTimer.stop();
			if (faceDetectedFlag)mMouseDlg->changePie(mMouseDlg->NEUTRAL);
			else mMouseDlg->changePie(mMouseDlg->LOSTFACE);
			mMouseDlg->RedrawWindow();
			smileMouseLocked = false;
			quickSmileUnlocked = true;
		}

	}
	else
	{
		if (mouseHookPause) mMouseDlg->changePie(mMouseDlg->PAUSE);
		else if (faceDetectedFlag)mMouseDlg->changePie(mMouseDlg->NEUTRAL);
		else mMouseDlg->changePie(mMouseDlg->LOSTFACE);
		mMouseDlg->RedrawWindow();
		smileMouseLocked = true;
		quickSmileUnlocked = true;
	}

}


void Tracker::timerMouseDlg()
{

	if (smileAIDetectedFlag || smileGeoDetectedFlag)
	{

		if (smileMouseLocked)
		{
			initiateStart();
			if (!mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_SHOW);
		}

		smileMouseLocked = false;


		double elapsedSeconds = mouseTimer.elapsedSeconds();

		if (elapsedSeconds < secDuration)
		{
			mMouseDlg->rotatePie(elapsedSeconds, false, false, secSmile, secToOneClickDuration, secToDoubleClickDuration, secDuration);

			// if keep smiling
			if (elapsedSeconds > secSmile)
			{
				// if one-click settings
				if (mMouseDlg->mouseClick == mMouseDlg->ONE_CL)
				{
					mouseClick = ONE_CL;
					if (elapsedSeconds > secToOneClickDuration)
						mouseClick = DO_NOTHING;

				}
				// if double-click settings
				else if (mMouseDlg->mouseClick == mMouseDlg->DOUBLE_CL)
				{
					mouseClick = ONE_CL;
					if (elapsedSeconds > secToDoubleClickDuration)
						mouseClick = DOUBLE_CL;
					if (elapsedSeconds > secToDoubleClickCancelDuration)
						mouseClick = DO_NOTHING;
				}
				// if left-click settings
				else if (mMouseDlg->mouseClick == mMouseDlg->RIGHT_CL)
				{
					mouseClick = RIGHT_CL;
					if (elapsedSeconds > secToOneClickDuration)
					mouseClick = DO_NOTHING;

				}
				else if (mMouseDlg->mouseClick == mMouseDlg->DRAG)
				{
					mouseClick = DRAG;
					if (elapsedSeconds > secToOneClickDuration)
						mouseClick = DO_NOTHING;

				}

			}

		}

		else mouseTimer.stop();
		mMouseDlg->RedrawWindow();

	}
	else
	{
		if (mouseClick != DO_NOTHING)
		{
			OnUnhookMouse();
			mMouseDlg->MouseInput();
			mouseClick = DO_NOTHING;
		}
		mouseTimer.stop();
		smileMouseLocked = true;

		if (mouseHookPause) mMouseDlg->changePie(mMouseDlg->PAUSE);
		else if (faceDetectedFlag)mMouseDlg->changePie(mMouseDlg->NEUTRAL);
		else mMouseDlg->changePie(mMouseDlg->LOSTFACE);
		mMouseDlg->RedrawWindow();
	}

}


void Tracker::dwellDetecting(float dist)
{
	if (dist < dwellDisp)
	{

		if (dwellMouseLocked)
		{
			dwellTimer.start();
			if (!mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_SHOW);
		}

		dwellMouseLocked = false;


		double elapsedSeconds = dwellTimer.elapsedSeconds();

		if (elapsedSeconds > secDwellStartTime)
		{
			if (elapsedSeconds < secDwellStartTime + dwellDuration)
			{
				mMouseDlg->rotatePie(elapsedSeconds - secDwellStartTime, true, false, secSmile, secToOneClickDuration, secToDoubleClickDuration, dwellDuration);
				mMouseDlg->RedrawWindow();
			}
			else
			{
				OnUnhookMouse();
				mMouseDlg->MouseInput();
				dwellTimer.stop();
				if (faceDetectedFlag)mMouseDlg->changePie(mMouseDlg->NEUTRAL);
				else mMouseDlg->changePie(mMouseDlg->LOSTFACE);
				mMouseDlg->RedrawWindow();
				dwellMouseLocked = true;
			}
		}

	}
	else
	{
		dwellTimer.stop();
		if (mouseHookPause) mMouseDlg->changePie(mMouseDlg->PAUSE);
		else if (faceDetectedFlag)mMouseDlg->changePie(mMouseDlg->NEUTRAL);
		else mMouseDlg->changePie(mMouseDlg->LOSTFACE);
		mMouseDlg->RedrawWindow();
		dwellMouseLocked = true;
	}

}



cv::Rect Tracker::calculateCrop(cv::Rect _crop, cv::Size _frame)
{

	if ((_crop.x + _crop.width) > _frame.width)
		_crop.x = _frame.width - _crop.width - 2;

	if ((_crop.y + _crop.height) > _frame.height)
		_crop.y = _frame.height - _crop.height - 2;

	if (_crop.x < 0)_crop.x = 2;
	if (_crop.y < 0)_crop.y = 2;

	return _crop;

}


void Tracker::initiateStart()
{
	secToOneClickDuration = secSmile + secOneClick;
	secToDoubleClickDuration = secSmile + secOneClick + secDoubleClick;
	secToOneClickCancelDuration = secSmile + secOneClick + secCancel;
	secToDoubleClickCancelDuration = secSmile + secOneClick + secDoubleClick + secCancel;
	if (mMouseDlg->mouseClick == mMouseDlg->ONE_CL) secDuration = secToOneClickCancelDuration;
	if (mMouseDlg->mouseClick == mMouseDlg->DOUBLE_CL) secDuration = secToDoubleClickCancelDuration;
	mouseTimer.start();
}