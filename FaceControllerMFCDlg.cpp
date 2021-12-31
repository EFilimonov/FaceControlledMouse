
// FaceControllerMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FaceControllerMFC.h"
#include "FaceControllerMFCDlg.h"
#include "afxdialogex.h"
#include "OptionsDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFaceControllerMFCApp

BEGIN_MESSAGE_MAP(CFaceControllerMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


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


void CFaceControllerMFCDlg::OnSetMousehook()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, NULL);
	//AfxMessageBox(L"");
}


void CFaceControllerMFCDlg::OnUnhookMouse()
{
	UnhookWindowsHookEx(MouseHook);
	OnSetMousehook();
}

void CFaceControllerMFCDlg::UnhookMouse()
{
	UnhookWindowsHookEx(MouseHook);
}


// CFaceControllerMFCDlg dialog



CFaceControllerMFCDlg::CFaceControllerMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACECONTROLLERMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

}

void CFaceControllerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, mCheckAutostart);
	DDX_Control(pDX, IDC_START, mButtonTracking);
	DDX_Control(pDX, IDOK, mButtonExit);
}

BEGIN_MESSAGE_MAP(CFaceControllerMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CFaceControllerMFCDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDOK, &CFaceControllerMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPTIONS, &CFaceControllerMFCDlg::OnBnClickedOptions)
	ON_MESSAGE(UWM_UPDATE_OPTIONS, OnOptionsUpdate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CFaceControllerMFCDlg::OnBnClickedCheckAutostart)
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_BUTTON_CLICK1X, &CFaceControllerMFCDlg::OnBnClickedButtonClick1x)
	ON_BN_CLICKED(IDC_BUTTON_CLICK2X, &CFaceControllerMFCDlg::OnBnClickedButtonClick2x)
	ON_BN_CLICKED(IDC_BUTTON_CLICKLEFT, &CFaceControllerMFCDlg::OnBnClickedButtonClickRight)
	ON_BN_CLICKED(IDC_BUTTON_GEO, &CFaceControllerMFCDlg::OnBnClickedButtonGeo)
	ON_BN_CLICKED(IDC_BUTTON_AI, &CFaceControllerMFCDlg::OnBnClickedButtonAi)
	ON_BN_CLICKED(IDC_BUTTON_DWELL, &CFaceControllerMFCDlg::OnBnClickedButtonDwell)
	ON_BN_CLICKED(IDC_BUTTON_TIMING, &CFaceControllerMFCDlg::OnBnClickedButtonTiming)
	ON_BN_CLICKED(IDC_BUTTON_OCR, &CFaceControllerMFCDlg::OnBnClickedButtonOcr)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, &CFaceControllerMFCDlg::OnBnClickedButtonDrag)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCMOUSEHOVER()
	ON_WM_NCMOUSELEAVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CFaceControllerMFCDlg::OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_CHROME, &CFaceControllerMFCDlg::OnBnClickedButtonChrome)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CFaceControllerMFCDlg message handlers

BOOL CFaceControllerMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon




	sPathToFolder = ExePath();


	/// <summary>
	/// change folder or remove path-patch for release!!!!
	/// </summary>
	/// <returns></returns>

	//sPathToFolder = "C:/Program Files (x86)/Face Controlled Mouse/";
	sPathToFolder = "";

	if (!enableMultithreading) cv::setNumThreads(1);

	readSerialized();

	this->GetClientRect(clientPosition);

	GetDlgItem(IDC_PICTURE)->MoveWindow(clientPosition.left, clientPosition.top, clientPosition.Width(), clientPosition.Height() - 80, true);


	bmp_click1x.LoadBitmap(IDB_BITMAP_CLICK1X);
	bmp_click2x.LoadBitmap(IDB_BITMAP_CLICK2X);
	bmp_clickRight.LoadBitmap(IDB_BITMAP_CLICKLEFT);
	bmp_off.LoadBitmap(IDB_BITMAP_OFF);
	bmp_settings.LoadBitmap(IDB_BITMAP_SETTINGS);
	bmp_play.LoadBitmap(IDB_BITMAP_PLAY);
	bmp_pause.LoadBitmap(IDB_BITMAP_PAUSE);
	bmp_stop.LoadBitmap(IDB_BITMAP_STOP);

	bmp_click1xp.LoadBitmap(IDB_BITMAP_CLICK1XP);
	bmp_click2xp.LoadBitmap(IDB_BITMAP_CLICK2XP);
	bmp_clickRightp.LoadBitmap(IDB_BITMAP_CLICKLEFTP);

	bmp_geo.LoadBitmap(IDB_BITMAP_GEO);
	bmp_ai.LoadBitmap(IDB_BITMAP_AI);
	bmp_dwell.LoadBitmap(IDB_BITMAP_DWELL);

	bmp_geop.LoadBitmap(IDB_BITMAP_GEOP);
	bmp_aip.LoadBitmap(IDB_BITMAP_AIP);
	bmp_dwellp.LoadBitmap(IDB_BITMAP_DWELLP);

	bmp_quickclick.LoadBitmap(IDB_BITMAP_QUICK);
	bmp_timerclick.LoadBitmap(IDB_BITMAP_TIMER);
	bmp_ABC.LoadBitmap(IDB_BITMAP_ABC);

	bmp_bigLogo.LoadBitmap(IDB_BITMAP_LOGOBIG);

	bmp_drag.LoadBitmap(IDB_BITMAP_DRAG);
	bmp_dragp.LoadBitmap(IDB_BITMAP_DRAGP);

	bmp_show.LoadBitmap(IDB_BITMAP_EYEOPEN);
	bmp_showp.LoadBitmap(IDB_BITMAP_EYEHIDE);

	bmp_chrome.LoadBitmap(IDB_BITMAP_CHROME);

	pButton_click1x = (CButton*)GetDlgItem(IDC_BUTTON_CLICK1X);
	pButton_click2x = (CButton*)GetDlgItem(IDC_BUTTON_CLICK2X);
	pButton_clickRight = (CButton*)GetDlgItem(IDC_BUTTON_CLICKLEFT);
	pButton_off = (CButton*)GetDlgItem(IDOK);
	pButton_settings = (CButton*)GetDlgItem(IDC_OPTIONS);
	pButton_start = (CButton*)GetDlgItem(IDC_START);
	pButton_geo = (CButton*)GetDlgItem(IDC_BUTTON_GEO);
	pButton_ai = (CButton*)GetDlgItem(IDC_BUTTON_AI);
	pButton_dwell = (CButton*)GetDlgItem(IDC_BUTTON_DWELL);
	pButton_timing = (CButton*)GetDlgItem(IDC_BUTTON_TIMING);
	pButton_ABC = (CButton*)GetDlgItem(IDC_BUTTON_OCR);
	pButton_Drag = (CButton*)GetDlgItem(IDC_BUTTON_DRAG);
	pButton_show = (CButton*)GetDlgItem(IDC_BUTTON_HIDE);
	pButton_chrome = (CButton*)GetDlgItem(IDC_BUTTON_CHROME);

	pBigLogoPic = (CStatic*)GetDlgItem(IDC_PICTURE);
	

	pButton_click1x->ModifyStyle(0, BS_BITMAP);
	pButton_click2x->ModifyStyle(0, BS_BITMAP);
	pButton_clickRight->ModifyStyle(0, BS_BITMAP);
	pButton_off->ModifyStyle(0, BS_BITMAP);
	pButton_settings->ModifyStyle(0, BS_BITMAP);
	pButton_start->ModifyStyle(0, BS_BITMAP);
	pButton_geo->ModifyStyle(0, BS_BITMAP);
	pButton_ai->ModifyStyle(0, BS_BITMAP);
	pButton_dwell->ModifyStyle(0, BS_BITMAP);
	pButton_timing->ModifyStyle(0, BS_BITMAP);
	pButton_ABC->ModifyStyle(0, BS_BITMAP);
	pButton_Drag->ModifyStyle(0, BS_BITMAP);
	pButton_show->ModifyStyle(0, BS_BITMAP);
	pButton_chrome->ModifyStyle(0, BS_BITMAP);

	pBigLogoPic->ModifyStyle(0, BS_BITMAP);

	///////////////////
	pButton_chrome->SetBitmap(bmp_chrome);
	pButton_click1x->SetBitmap(bmp_click1xp);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_clickRight->SetBitmap(bmp_clickRight);

	if (trackerPtr->mMouseDlg->mouseClick == trackerPtr->mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
	else if (trackerPtr->mMouseDlg->mouseClick == trackerPtr->mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
	else if (trackerPtr->mMouseDlg->mouseClick == trackerPtr->mMouseDlg->RIGHT_CL)OnBnClickedButtonClickRight();


	if (trackerPtr->showVideoFlag)
		pButton_show->SetBitmap(bmp_show);
	else pButton_show->SetBitmap(bmp_showp);

	pButton_ai->SetBitmap(bmp_ai);
	pButton_geo->SetBitmap(bmp_geo);
	pButton_dwell->SetBitmap(bmp_dwell);

	if (trackerPtr->detectDwellFlag)
	{
		pButton_dwell->SetBitmap(bmp_dwellp);
		pButton_ai->SetBitmap(bmp_ai);
		pButton_geo->SetBitmap(bmp_geo);
	}
	else
	{
		if (trackerPtr->detectSmileGeoFlag) 
		{ 
			pButton_geo->SetBitmap(bmp_geop);
			pButton_dwell->SetBitmap(bmp_dwell);
		}
		if (trackerPtr->detectSmileAIFlag) 
		{ 
			pButton_ai->SetBitmap(bmp_aip);
			pButton_dwell->SetBitmap(bmp_dwell);
		}
	}



	if (trackerPtr->isQuickClick)pButton_timing->SetBitmap(bmp_quickclick);
	else pButton_timing->SetBitmap(bmp_timerclick);


	///////////////////
	pButton_off->SetBitmap(bmp_off);
	pButton_settings->SetBitmap(bmp_settings);
	pButton_start->SetBitmap(bmp_play);
	pButton_ABC->SetBitmap(bmp_ABC);
	pButton_Drag->SetBitmap(bmp_drag);



	pBigLogoPic->SetBitmap(bmp_bigLogo);



	bmp_bigLogo.SetBitmapDimension(clientPosition.Width(), clientPosition.Height());
	pBigLogoPic->MoveWindow(clientPosition.left, clientPosition.top+80, clientPosition.Width(), clientPosition.Height() - 80, true);

	mCheckAutostart.SetCheck(TRUE);

	GetWindowRect(maxRect);

	// initiate minimize variables
	minWidth = maxRect.Width();
	maxHeight = maxRect.Height();

	CRect bt;
	CWnd* wnd = GetDlgItem(IDOK);
	wnd->GetWindowRect(&bt);
	ScreenToClient(bt);

	RECT w, c;
	GetWindowRect(&w);
	GetClientRect(&c);
	int titleHeght = (w.bottom - w.top) - (c.bottom - c.top);

	btHeight = bt.Height();

	minHeight = btHeight + titleHeght;

	startwnd = GetDlgItem(IDC_START);

	if (!trackerPtr->showVideoFlag) minimizeW();
	else maximizeW();
	//////////


	if (isAutostart)
	{
		SetTimer(ID_TIMER_1, 11000, NULL);
		SetTimer(ID_TIMER_2, 1000, NULL);
	}



	//std::string cascadeName = "haarcascade_frontalface_alt2.xml";
	std::string cascadeName = sPathToFolder + "haarcascade_frontalface_alt2.xml";



	trackerPtr->loadedFrontalfaceModelFlag = trackerPtr->frontalFaceDetector.load(cascadeName);
	if (!trackerPtr->loadedFrontalfaceModelFlag)
	{
		MessageBox(_T("Please, add haarcascade_frontalface_alt2.xml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	//cascadeName = "haarcascade_smile.xml";
	cascadeName = sPathToFolder + "haarcascade_smile.xml";
	trackerPtr->loadedSmileModelFlag = trackerPtr->smileDetector.load(cascadeName);
	if (!trackerPtr->loadedSmileModelFlag)
	{
		MessageBox(_T("Please, add haarcascade_smile.xml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	// Create an instance of Facemark
	trackerPtr->facemark = cv::face::FacemarkLBF::create();

	// Load landmark detector
	//trackerPtr->facemark->loadModel("lbfmodel.yaml");
	trackerPtr->facemark->loadModel(sPathToFolder + "lbfmodel.yaml");
	trackerPtr->loadedFacemarkModelFlag = !trackerPtr->facemark->empty();
	if (!trackerPtr->loadedFacemarkModelFlag)
	{
		MessageBox(_T("Please, add lbfmodel.yaml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	bool openFlag = true;



	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*

*/

void CFaceControllerMFCDlg::resizeCVWindow()
{
	GetClientRect(clientPosition);
	newHeight = clientPosition.Height() - btHeight;
	newTop = clientPosition.top + btHeight;
	newWidth = newHeight * aspectRatio;
	newLeft = clientPosition.left + (clientPosition.Width() - newWidth) / 2;
	pBigLogoPic->MoveWindow(newLeft, newTop, newWidth, newHeight, true);

	if (trackerPtr->allowResize) 
		cv::resizeWindow("view", newWidth, newHeight);

	GetWindowRect(maxRect);
}


void CFaceControllerMFCDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{	
		CDialogEx::OnPaint();
	}

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFaceControllerMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFaceControllerMFCDlg::OnBnClickedStart()
{
	isAutostart = false;
	force1Click();

	KillTimer(ID_TIMER_1);
	KillTimer(ID_TIMER_2);
	GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
	GetDlgItem(IDC_CHECK_AUTOSTART)->EnableWindow(0);


	if (!trackerPtr->openFlag)
	{
		MessageBox(_T("Please add the required files to the folder"));
		return;
	}


	if (!trackerPtr->cap.isOpened())
	{ 
		for (int i = 0; i < 2402; i++)
		{
			trackerPtr->cap.open(i);
			if (trackerPtr->cap.isOpened()) break;
			else trackerPtr->cap.release();
		}
		if (!trackerPtr->cap.isOpened())
		{
			trackerPtr->cap.release();
			MessageBox(_T("Please connect your webcam"));

			return;
		}
	}

	if (trackerPtr->buttonStop)
	{
		trackerPtr->buttonStop = false;
		pButton_start->SetBitmap(bmp_pause);
		trackerPtr->turnOffClick = false;
		if (!trackerPtr->mMouseDlg->IsWindowVisible()) trackerPtr->mMouseDlg->ShowWindow(SW_SHOW);
		trackerPtr->mMouseDlg->click = trackerPtr->mMouseDlg->NEUTRAL;
		trackerPtr->mMouseDlg->mStaticInfoText = (L"\U0001F60E");// sunglass

	}
	else
	{
		trackerPtr->buttonStop = true;
		pButton_start->SetBitmap(bmp_play);
		trackerPtr->turnOffClick = true;
		OnUnhookMouse();
		trackerPtr->mMouseDlg->ShowWindow(SW_HIDE);
	}

	if (needTrackerInitiate)
	{
		


		cv::namedWindow("view", cv::WINDOW_KEEPRATIO);

		HWND hChild = (HWND)cvGetWindowHandle("view");
		HWND hParent = ::GetParent(hChild);

		::SetParent(hChild, GetDlgItem(IDC_PICTURE)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);

		CRect bitmapPosition;
		GetDlgItem(IDC_PICTURE)->GetWindowRect(bitmapPosition);

		aspectRatio = (float)trackerPtr->cap.get(cv::CAP_PROP_FRAME_WIDTH) / (float)trackerPtr->cap.get(cv::CAP_PROP_FRAME_HEIGHT);

		GetClientRect(clientPosition);
		newHeight = clientPosition.Height() - btHeight;
		newTop = clientPosition.top + btHeight;
		newWidth = newHeight * aspectRatio;
		newLeft = clientPosition.left + (clientPosition.Width() - newWidth) / 2;
		pBigLogoPic->MoveWindow(newLeft, newTop, newWidth, newHeight, true);

		cv::resizeWindow("view", newWidth, newHeight);

		trackerPtr->allowResize = true;

		OnPaint();

		if (needTrackerInitiate) trackerPtr->initiateTracker();

		needTrackerInitiate = false;

		
		// Moving mouse thread
		std::thread t1(thread_proc1, this);
		t1.detach();
		
		// Face detection thread
		std::thread t2(thread_proc2, this);
		t2.detach();

		OnSetMousehook();

		////////////
		// main flow
		////////////
		for (;;)
		{

			// set hook to detect mouse move///////

			// ignore tracker action
			if (trackerPtr->skipHook)
			{
				trackerPtr->skipHook = false;
				isGlobalMouseMove = false;
			}
			else if (isGlobalMouseMove)// if physical mouse moving
			{
				mMouseDlg->ShowWindow(SW_HIDE);
				// stop monitoring dwell
				mMouseDlg->dwellTimer.stop();
				// set timer
				SetTimer(ID_TIMER_MOVELOCK, pauseTime*1000, NULL);
				// tracker doesn't move cursor
				trackerPtr->mouseHookPause = true;
				// doesn't click
				trackerPtr->turnOffClick = true;
				// reset flag
				isGlobalMouseMove = false;
			}

			if (trackerPtr->mouseHookPause)
			{
				OnUnhookMouse();
			}
			/////////////////////////////////////////


			// possibility to start even when clicks stopped/////////
			if (trackerPtr->buttonStop)
			{
				startwnd->GetWindowRect(buttonStartPosition);
				GetCursorPos(&cursorPos);
				if (cursorPos.y > buttonStartPosition.top && cursorPos.y < buttonStartPosition.bottom
					&& cursorPos.x > buttonStartPosition.left && cursorPos.x < buttonStartPosition.right)
				{
					mMouseDlg->dwellDetecting(0);
				}
				else
				{
					mMouseDlg->dwellMouseLocked = true;
					if (mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_HIDE);
				}
			}
			/////////////////////////////////////////////////////////

			// TRACKING
			if (!trackerPtr->Tracking()) break;

		} 

		UnhookMouse();
	}

}

// Moving mouse
void CFaceControllerMFCDlg::MouseActions()
{
	while (trackerPtr->openFlag)
	{

			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			if (!trackerPtr->mouseHookPause)
			{
				mMouseDlg->moveMouseDlg(trackerPtr->mouseX, trackerPtr->mouseY);
			}
	}
}

// Face detection
void CFaceControllerMFCDlg::TrackingActions()
{
	while (trackerPtr->openFlag)
	{
		trackerPtr->preTrackingActions();
		if(trackerPtr->faceDetectedFlag) std::this_thread::sleep_for(std::chrono::milliseconds(faceFrames*100));
		else std::this_thread::sleep_for(std::chrono::milliseconds(faceFrames * 20));
	}
}

void CFaceControllerMFCDlg::OnBnClickedOk()
{
	serializeChanges();
	trackerPtr->openFlag = false;
	trackerPtr->cap.release();
	UnhookMouse();
	CDialogEx::OnOK();
}

void CFaceControllerMFCDlg::OnClose()
{
	serializeChanges();
	trackerPtr->openFlag = false;
	trackerPtr->cap.release();
	UnhookMouse();
	CDialogEx::OnClose();

}


void CFaceControllerMFCDlg::OnBnClickedOptions()
{
	if (!isAutostart) force1Click();

	if (!mOptionsDlg.GetSafeHwnd()) // Have we created the dialog?
		mOptionsDlg.Create(IDD_OPTIONS, this);

	initSettings();


	// show on left or right side?

	CRect ScreenRect;

	GetWindowRect(ScreenRect);

	int screenHorCenter = (int)ScreenRect.Width() / 2;
	int screenVertCenter = (int)ScreenRect.Height() / 2;


	CRect rect;
	mOptionsDlg.GetWindowRect(rect); // child window w/h
	int dx = rect.Width();
	int dy = rect.Height();

	GetWindowRect(rect);

	if ((int)(rect.left + rect.right) / 2 <= screenHorCenter)
	{
		rect.left = rect.right;
		rect.right = rect.right + dx;
		rect.bottom = rect.top + dy;
		mOptionsDlg.MoveWindow(rect);
	}
	else
	{
		rect.right = rect.left;
		rect.left = rect.left - dx;
		rect.bottom = rect.top + dy;
		mOptionsDlg.MoveWindow(rect);

	}

	if (!mOptionsDlg.IsWindowVisible()) // Is it visible?
		mOptionsDlg.ShowWindow(SW_SHOW);
	else mOptionsDlg.ShowWindow(SW_HIDE);


}



LRESULT CFaceControllerMFCDlg::OnOptionsUpdate(WPARAM wparam, LPARAM lparam)
{
	CString* pstr = (CString*)wparam;

	return 0;
}


		//	MessageBox(_T("Please, add haarcascade_frontalface_alt2.xml file to the programm folder"));


LRESULT CFaceControllerMFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{

	case UWM_CUSTOMCLICK:
		trackerPtr->skipHook = true;
		return 0;

	/*case UWM_CUSTOMSTARTTRACK:
			startwnd->GetWindowRect(trackerPtr->buttonStartPosition);
		return 0;*/

	case UWM_CUSTOMRIGHTCLICK:
		if (trackerPtr->mMouseDlg->prevRightClick == trackerPtr->mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
		if (trackerPtr->mMouseDlg->prevRightClick == trackerPtr->mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
		return 0;

	case UWM_CUSTOMDRAGMAIN:


		GetWindowRect(&w);

		if (((w.top + trackerPtr->correctedDY) > 1) && ((w.bottom + trackerPtr->correctedDY) < trackerPtr->vertLimit))
		{
			w.top += trackerPtr->correctedDY;
			w.bottom += trackerPtr->correctedDY;
		}
		if (((w.right + trackerPtr->correctedDX) < trackerPtr->horLimit) && ((w.left + trackerPtr->correctedDX) > 1))
		{
			w.left += trackerPtr->correctedDX;
			w.right += trackerPtr->correctedDX;
		}
		MoveWindow(&w);
		OnPaint();


		return 0;


	case UWM_CUSTOMDRAGOPTION:


		mOptionsDlg.GetWindowRect(&w);

		if (((w.top + trackerPtr->correctedDY) > 1) && ((w.bottom + trackerPtr->correctedDY) < trackerPtr->vertLimit))
		{
			w.top += trackerPtr->correctedDY;
			w.bottom += trackerPtr->correctedDY;
		}
		if (((w.right + trackerPtr->correctedDX) < trackerPtr->horLimit) && ((w.left + trackerPtr->correctedDX) > 1))
		{
			w.left += trackerPtr->correctedDX;
			w.right += trackerPtr->correctedDX;
		}
		mOptionsDlg.MoveWindow(&w);
		mOptionsDlg.OnPaint();


		return 0;

	case UWM_UPDATE_OPTIONS:
		switch (wParam)
		{
			case IDC_CHECK_THREADING:
				enableMultithreading = mOptionsDlg.pCAdvancedTab.enableMultithreading;
				if (!enableMultithreading) cv::setNumThreads(1);
				else cv::setNumThreads(-1);

			return 0;

			case IDC_COMBO_LANGUAGE:
				changeLanguage(mOptionsDlg.pCAdvancedTab.langNum);
				langNum = mOptionsDlg.pCAdvancedTab.langNum;

			return 0;

			case IDC_CHECK_SOUND:
				mMouseDlg->needSound = mOptionsDlg.pCAdvancedTab.needSound;
			return 0;

			case IDC_SLIDER_ONINPUT:
				pauseTime = mOptionsDlg.pCAdvancedTab.fSliderOnInput;
				return 0;

			case IDC_CHECK_AUTOSTART:
				needAutostart = mOptionsDlg.pCAdvancedTab.needAutostart;
				return 0;

			case IDC_SLIDER_CUSTOM:
				trackerPtr->minDistRatio = (double) mOptionsDlg.pCAdvancedTab.iSliderCustom;
				trackerPtr->needToTrackerInit = true;
				return 0;

			case IDC_SLIDER_FACE_FRAMES:
				faceFrames = mOptionsDlg.pCAdvancedTab.iSliderFaceFrames - 1;
			return 0;

			case IDC_SLIDER_FACE_MINNUM:
				trackerPtr->minCornersCount = mOptionsDlg.pCAdvancedTab.iSliderMinNum;
				trackerPtr->maxCornersCount = (int)trackerPtr->minCornersCount * 1.7;
				trackerPtr->needToTrackerInit = true;
			return 0;

			case IDC_SLIDER_SPEED_HORIZONTAL:
				trackerPtr->horSensitivity = mOptionsDlg.pCMainTab.fSliderSpeedHorisontal;
			return 0;

			case IDC_SLIDER_SPEED_VERTICAL:
				trackerPtr->verSensitivity = mOptionsDlg.pCMainTab.fSliderSpeedVertical;
			return 0;

			case IDC_SLIDER_SMILE_ANGLE:
				trackerPtr->smilingTriggerAngle = mOptionsDlg.pCMainTab.iSliderSmileAngle;
			return 0;

			case IDC_SLIDER_SMILE_SENSITIVITY:
				trackerPtr->minNeighborsSmileDetector = mOptionsDlg.pCMainTab.iSliderSmileSensitivity;
				return 0;

			case IDC_SLIDER_DWELL_DISPL:
				mMouseDlg->dwellDisp = mOptionsDlg.pCMainTab.fSliderDwellDispl;
				return 0;

			case IDC_SLIDER_DWELL_TIME:
				mMouseDlg->dwellDuration = mOptionsDlg.pCMainTab.fSliderDwellTime;
				return 0;

			case IDC_SLIDER_QUICK_TIME:
				mMouseDlg->secQuickClick = mOptionsDlg.pCMainTab.fSliderQuickTime;
				return 0;

			case IDC_SLIDER_1X:
				mMouseDlg->secSmile = mOptionsDlg.pCMainTab.fSlider1x;
				return 0;

			case IDC_SLIDER_2X: 
				mMouseDlg->secOneClick = mOptionsDlg.pCMainTab.fSlider2x;
				return 0;

			case IDC_SLIDER_CANCEL:
				mMouseDlg->secDoubleClick = mOptionsDlg.pCMainTab.fSliderCancel;
				return 0;

			case IDC_SLIDER_TIMETODWELL:
				mMouseDlg->secDwellStartTime = mOptionsDlg.pCMainTab.fSliderTimeToDwell;
				return 0;
			
			case IDC_CHECK_FLIPCAMERA:
				trackerPtr->flipCameraFlag = mOptionsDlg.pCAdvancedTab.flipCameraFlag;
				return 0;
				
			case IDC_SLIDER_EWMA:
				trackerPtr->ewmaAlpha = mOptionsDlg.pCAdvancedTab.fSliderEWMA;
				trackerPtr->needToTrackerInit = true;
				return 0;

/*			case IDC_SLIDER_MARKS:
				trackerPtr->accumMoveRatio = mOptionsDlg.pCAdvancedTab.fSliderMarks;
				trackerPtr->needToTrackerInit = true;
				return 0;*/

			case IDC_SLIDER_ACCELERATION:
				trackerPtr->velocitySlider = mOptionsDlg.pCAdvancedTab.fSliderAcceleration;
				trackerPtr->velocityK = trackerPtr->velocitySlider * 0.002 - 0.0014;
				return 0;

			case IDC_SLIDER_FACE_SENSITIVITY:
				trackerPtr->minFaceNeighbors = mOptionsDlg.pCAdvancedTab.iSliderFace;
				return 0;

			case IDC_EDIT_KEYBOARD:
				trackerPtr->editKeyboard = mOptionsDlg.pCAdvancedTab.mEditKeyboard;
				return 0;

			case IDC_CHECK_EQUALIZE:
				trackerPtr->needEqualize = mOptionsDlg.pCAdvancedTab.needEqualize;
				return 0;

			case IDCANCEL:
				trackerPtr->horSensitivity = mOptionsDlg.back_fSliderSpeedHorisontal;
				trackerPtr->verSensitivity = mOptionsDlg.back_fSliderSpeedVertical;
				trackerPtr->smilingTriggerAngle = mOptionsDlg.back_iSliderSmileAngle;
				trackerPtr->minNeighborsSmileDetector = mOptionsDlg.back_iSliderSmileSensitivity;
				mMouseDlg->dwellDisp = mOptionsDlg.back_fSliderDwellDispl;
				mMouseDlg->dwellDuration = mOptionsDlg.back_fSliderDwellTime;
				mMouseDlg->secQuickClick = mOptionsDlg.back_fSliderQuickTime;
				mMouseDlg->secSmile = mOptionsDlg.back_fSlider1x;
				mMouseDlg->secOneClick = mOptionsDlg.back_fSlider2x;
				mMouseDlg->secDoubleClick = mOptionsDlg.back_fSliderCancel;
				mMouseDlg->secDwellStartTime = mOptionsDlg.back_fSliderTimeToDwell;
				trackerPtr->mMouseDlg->needSound = mOptionsDlg.back_needSound;

				trackerPtr->flipCameraFlag = mOptionsDlg.back_flipCameraFlag;
				trackerPtr->needEqualize = mOptionsDlg.back_needEqualize;
				trackerPtr->ewmaAlpha = mOptionsDlg.back_fSliderEWMA;
				trackerPtr->velocitySlider = mOptionsDlg.back_fSliderAcceleration;
				trackerPtr->minFaceNeighbors = mOptionsDlg.back_iSliderFace;
				trackerPtr->editKeyboard = mOptionsDlg.back_mEditKeyboard;
				pauseTime = mOptionsDlg.back_fSliderOnInput;
				faceFrames = mOptionsDlg.back_iSliderFaceFrames;
				trackerPtr->minCornersCount = mOptionsDlg.back_iSliderMinNum;


				return 0;

			case IDC_APPLY:

				onApplyActions();

				return 0;

			case IDOK:

				onApplyActions();

				return 0;

			case IDC_BUTTON_RESET:

				resetDefaults();
				initSettings();

				return 0;

		}

	return 0;

	case WM_TIMER:

		switch (wParam)
		{
		case ID_TIMER_1:

			if (isAutostart)
			{
				isAutostart = false;
				GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(autostartStr);
				GetDlgItem(IDC_CHECK_AUTOSTART)->EnableWindow(0);
				OnBnClickedStart();
				KillTimer(ID_TIMER_1);
				KillTimer(ID_TIMER_2);

			}
			else
			{
				GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(autostartStr);
				KillTimer(ID_TIMER_1);
				KillTimer(ID_TIMER_2);
			}
			isAutostart = false;

			return 0;

		case ID_TIMER_2:


			pButton_click1x->RedrawWindow();
			pButton_click2x->RedrawWindow();
			pButton_clickRight->RedrawWindow();
			pButton_off->RedrawWindow();
			pButton_settings->RedrawWindow();
			pButton_start->RedrawWindow();
			pButton_geo->RedrawWindow();
			pButton_ai->RedrawWindow();
			pButton_dwell->RedrawWindow();
			pButton_timing->RedrawWindow();
			pButton_ABC->RedrawWindow();
			pButton_Drag->RedrawWindow();
			pButton_show->RedrawWindow();

			if (isAutostart)
			{
				autostartSeconds++;
				int elapsedSeconds = ID_TIMER_1 / 1000 - autostartSeconds;
				if (elapsedSeconds >= 0)
				{
					strAutostartSeconds.Format(_T("%d"), elapsedSeconds);
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(autostartStr + strAutostartSeconds + _T(" s"));
				}
				else
				{
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(autostartStr);
					pButton_start->SetBitmap(bmp_pause);
				}
				SetTimer(ID_TIMER_2, 1000, NULL);

			}
			else
			{
				KillTimer(ID_TIMER_1);
				KillTimer(ID_TIMER_2);

			}
			return 0;

		case ID_TIMER_2CLICKS:
			trackerPtr->mMouseDlg->isTimer2Clicks = false;
			KillTimer(ID_TIMER_2CLICKS);

			return 0;

		case ID_TIMER_MOVELOCK:
			trackerPtr->mouseHookPause = false;
			mMouseDlg->changePie(mMouseDlg->NEUTRAL);
			if (!mMouseDlg->IsWindowVisible()) mMouseDlg->ShowWindow(SW_SHOW);
			KillTimer(ID_TIMER_MOVELOCK);
			if (!trackerPtr->buttonStop) trackerPtr->turnOffClick = false;

			return 0;

		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CFaceControllerMFCDlg::onApplyActions()
{
	mOptionsDlg.back_fSliderSpeedHorisontal = trackerPtr->horSensitivity;
	mOptionsDlg.back_fSliderSpeedVertical = trackerPtr->verSensitivity;
	mOptionsDlg.back_iSliderSmileAngle = trackerPtr->smilingTriggerAngle;
	mOptionsDlg.back_iSliderSmileSensitivity = trackerPtr->minNeighborsSmileDetector;
	mOptionsDlg.back_fSliderDwellDispl = mMouseDlg->dwellDisp;
	mOptionsDlg.back_fSliderDwellTime = mMouseDlg->dwellDuration;
	mOptionsDlg.back_fSliderQuickTime = mMouseDlg->secQuickClick;
	mOptionsDlg.back_fSlider1x = mMouseDlg->secSmile;
	mOptionsDlg.back_fSlider2x = mMouseDlg->secOneClick;
	mOptionsDlg.back_fSliderCancel = mMouseDlg->secDoubleClick;
	mOptionsDlg.back_fSliderTimeToDwell = mMouseDlg->secDwellStartTime;
	mOptionsDlg.back_needSound = trackerPtr->mMouseDlg->needSound;
	mOptionsDlg.back_iSliderFaceFrames = faceFrames + 1;
	mOptionsDlg.back_iSliderMinNum = trackerPtr->minCornersCount;
	mOptionsDlg.back_flipCameraFlag = trackerPtr->flipCameraFlag;
	mOptionsDlg.back_needEqualize = trackerPtr->needEqualize;
	mOptionsDlg.back_fSliderEWMA = trackerPtr->ewmaAlpha;
	mOptionsDlg.back_fSliderAcceleration = trackerPtr->velocitySlider;
	mOptionsDlg.back_iSliderFace = trackerPtr->minFaceNeighbors;
	mOptionsDlg.back_mEditKeyboard = trackerPtr->editKeyboard;
	mOptionsDlg.back_fSliderOnInput = pauseTime;

}


void CFaceControllerMFCDlg::OnBnClickedCheckAutostart()
{
	if (!isAutostart) force1Click();

	UpdateData(TRUE);
	if (mCheckAutostart.GetCheck() == NULL)
	{
		GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(autostartStr);
		KillTimer(ID_TIMER_1);
		KillTimer(ID_TIMER_2);
		GetDlgItem(IDC_CHECK_AUTOSTART)->EnableWindow(0);
		isAutostart = false;
	}
}



void CFaceControllerMFCDlg::OnBnClickedButtonGeo()
{
	if (!isAutostart) force1Click();

	if (trackerPtr->detectSmileGeoFlag == true && trackerPtr->detectSmileAIFlag == true)
	{
		trackerPtr->detectSmileGeoFlag = false;
		pButton_geo->SetBitmap(bmp_geo);
		if (trackerPtr->detectDwellFlag)trackerPtr->detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_geo->RedrawWindow();
	}
	else if (trackerPtr->detectSmileGeoFlag == true && trackerPtr->detectSmileAIFlag == true)
	{

	}
	else
	{ 
		trackerPtr->detectSmileGeoFlag = true;
		pButton_geo->SetBitmap(bmp_geop);
		if (trackerPtr->detectDwellFlag)trackerPtr->detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_geo->RedrawWindow();

	}
}


void CFaceControllerMFCDlg::OnBnClickedButtonAi()
{
	if (!isAutostart) force1Click();
	if (trackerPtr->detectSmileGeoFlag == true && trackerPtr->detectSmileAIFlag == true)
	{
		trackerPtr->detectSmileAIFlag = false;
		pButton_ai->SetBitmap(bmp_ai);
		if (trackerPtr->detectDwellFlag)trackerPtr->detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_ai->RedrawWindow();
	}
	else
	{
		trackerPtr->detectSmileAIFlag = true;
		pButton_ai->SetBitmap(bmp_aip);
		if (trackerPtr->detectDwellFlag)trackerPtr->detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_ai->RedrawWindow();
	}
}



void CFaceControllerMFCDlg::OnBnClickedButtonDwell()
{
	if (!isAutostart) force1Click();
	trackerPtr->detectDwellFlag = true;
	pButton_dwell->SetBitmap(bmp_dwellp);
	if (trackerPtr->detectSmileAIFlag) trackerPtr->detectSmileAIFlag = false;
	if (trackerPtr->detectSmileGeoFlag) trackerPtr->detectSmileGeoFlag = false;
	pButton_ai->SetBitmap(bmp_ai);
	pButton_geo->SetBitmap(bmp_geo);
	pButton_dwell->RedrawWindow();
	pButton_ai->RedrawWindow();
	pButton_geo->RedrawWindow();
}



void CFaceControllerMFCDlg::OnBnClickedButtonClick1x()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1xp);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	trackerPtr->mMouseDlg->mouseClick = trackerPtr->mMouseDlg->ONE_CL;
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
	trackerPtr->mMouseDlg->prevRightClick = trackerPtr->mMouseDlg->ONE_CL;

}


void CFaceControllerMFCDlg::OnBnClickedButtonClick2x()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2xp);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	trackerPtr->mMouseDlg->mouseClick = trackerPtr->mMouseDlg->DOUBLE_CL;
	mMouseDlg->secDuration = mMouseDlg->secToDoubleClickCancelDuration;
	trackerPtr->mMouseDlg->prevRightClick = trackerPtr->mMouseDlg->DOUBLE_CL;
}


void CFaceControllerMFCDlg::OnBnClickedButtonClickRight()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRightp);
	trackerPtr->mMouseDlg->mouseClick = trackerPtr->mMouseDlg->RIGHT_CL;
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
}

void CFaceControllerMFCDlg::OnBnClickedButtonTiming()
{
	if (!isAutostart) force1Click();

	if (trackerPtr->isQuickClick)
	{
		pButton_timing->SetBitmap(bmp_timerclick);
	}
	else
	{
		pButton_timing->SetBitmap(bmp_quickclick);
	}
	trackerPtr->isQuickClick = !trackerPtr->isQuickClick;
}

void CFaceControllerMFCDlg::force1Click()
{
	if (trackerPtr->isQuickClick)
	{
		trackerPtr->mMouseDlg->isTimer2Clicks = true;
		SetTimer(ID_TIMER_2CLICKS, 200, NULL);
	}
}


void CFaceControllerMFCDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	resizeCVWindow();


		
}

// save main dialog parameters
void CFaceControllerMFCDlg::serializeChanges()
{
	cv::FileStorage fs(sPathToFolder + "config.xml", cv::FileStorage::WRITE);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open config.xml file, your changes are not saved!"), NULL);
	else
	{
		fs << "detectSmileGeoFlag" << trackerPtr->detectSmileGeoFlag;
		fs << "detectSmileAIFlag" << trackerPtr->detectSmileAIFlag;
		fs << "detectDwellFlag" << trackerPtr->detectDwellFlag;
		if (trackerPtr->mMouseDlg->mouseClick == trackerPtr->mMouseDlg->DRAG)trackerPtr->mMouseDlg->mouseClick = trackerPtr->mMouseDlg->ONE_CL;
		fs << "mouseClick" << trackerPtr->mMouseDlg->mouseClick;
		fs << "isQuickClick" << trackerPtr->isQuickClick;
		fs << "horSensitivity" << trackerPtr->horSensitivity;
		fs << "verSensitivity" << trackerPtr->verSensitivity;
		fs << "smilingTriggerAngle" << trackerPtr->smilingTriggerAngle;
		fs << "minNeighborsSmileDetector" << trackerPtr->minNeighborsSmileDetector;
		fs << "dwellDisp" << mMouseDlg->dwellDisp;
		fs << "dwellDuration" << mMouseDlg->dwellDuration;
		fs << "secQuickClick" << mMouseDlg->secQuickClick;
		fs << "secSmile" << mMouseDlg->secSmile;
		fs << "secOneClick" << mMouseDlg->secOneClick;
		fs << "secDoubleClick" << mMouseDlg->secDoubleClick;
		fs << "secDwellStartTime" << mMouseDlg->secDwellStartTime;
		fs << "flipCameraFlag" << trackerPtr->flipCameraFlag;
		fs << "showVideoFlag" << trackerPtr->showVideoFlag;
		fs << "ewmaAlpha" << trackerPtr->ewmaAlpha;
//		fs << "accumMoveRatio" << trackerPtr->accumMoveRatio;
		fs << "velocitySlider" << trackerPtr->velocitySlider;
		fs << "minFaceNeighbors" << trackerPtr->minFaceNeighbors;
		std::string strStd = CT2A(trackerPtr->editKeyboard);
		ek = strStd;
		fs << "editKeyboard" << ek;
		fs << "needEqualize" << trackerPtr->needEqualize;
		fs << "needAutostart" << needAutostart;
		fs << "minDistRatio" << trackerPtr->minDistRatio;
		fs << "minCornersCount" << trackerPtr->minCornersCount;
		fs << "faceFrames" << faceFrames;
		fs << "pauseTime" << pauseTime;
		fs << "needSound" << trackerPtr->mMouseDlg->needSound;
		fs << "langNum" << langNum;
		fs << "enableMultithreading" << enableMultithreading;
	}
	fs.release();
}


//load
void CFaceControllerMFCDlg::readSerialized()
{
	cv::FileStorage fs(sPathToFolder + "config.xml", cv::FileStorage::READ);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open config.xml file for reading!"), NULL);
	else
	{
		fs["detectSmileGeoFlag"] >> trackerPtr->detectSmileGeoFlag;
		fs["detectSmileAIFlag"] >> trackerPtr->detectSmileAIFlag;
		fs["detectDwellFlag"] >> trackerPtr->detectDwellFlag;
		fs["mouseClick"] >> trackerPtr->mMouseDlg->mouseClick;
		fs["isQuickClick"] >> trackerPtr->isQuickClick;
		fs["horSensitivity"] >> trackerPtr->horSensitivity;
		fs["verSensitivity"] >> trackerPtr->verSensitivity;
		fs["smilingTriggerAngle"] >> trackerPtr->smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> trackerPtr->minNeighborsSmileDetector;
		fs["dwellDisp"] >> mMouseDlg->dwellDisp;
		fs["dwellDuration"] >> mMouseDlg->dwellDuration;
		fs["secQuickClick"] >> mMouseDlg->secQuickClick;
		fs["secSmile"] >> mMouseDlg->secSmile;
		fs["secOneClick"] >> mMouseDlg->secOneClick;
		fs["secDoubleClick"] >> mMouseDlg->secDoubleClick;
		fs["secDwellStartTime"] >> mMouseDlg->secDwellStartTime;;
		fs["flipCameraFlag"] >> trackerPtr->flipCameraFlag;
		fs["showVideoFlag"] >> trackerPtr->showVideoFlag;
		fs["ewmaAlpha"] >> trackerPtr->ewmaAlpha;
//		fs["accumMoveRatio"] >> trackerPtr->accumMoveRatio;
		fs["velocitySlider"] >> trackerPtr->velocitySlider;
		fs["minFaceNeighbors"] >> trackerPtr->minFaceNeighbors;
		fs["editKeyboard"] >> ek; trackerPtr->editKeyboard= ek.c_str(); mOptionsDlg.back_mEditKeyboard = trackerPtr->editKeyboard;
		fs["needEqualize"] >>  trackerPtr->needEqualize;
		fs["needAutostart"] >> needAutostart;
		fs["minDistRatio"] >> trackerPtr->minDistRatio;
		fs["minCornersCount"] >> trackerPtr->minCornersCount;
		fs["faceFrames"] >> faceFrames;
		fs["pauseTime"] >> pauseTime;
		fs["needSound"] >> trackerPtr->mMouseDlg->needSound;
		fs["langNum"] >> langNum;
		fs["enableMultithreading"] >> enableMultithreading;
	}
	fs.release();
}



//reset
void CFaceControllerMFCDlg::resetDefaults()
{
	cv::FileStorage fs(sPathToFolder + "default.xml", cv::FileStorage::READ);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open default.xml file, your changes are not saved!"), NULL);
	else
	{
		fs["detectSmileGeoFlag"] >> trackerPtr->detectSmileGeoFlag;
		fs["detectSmileAIFlag"] >> trackerPtr->detectSmileAIFlag;
		fs["detectDwellFlag"] >> trackerPtr->detectDwellFlag;
		fs["mouseClick"] >> trackerPtr->mMouseDlg->mouseClick;
		fs["isQuickClick"] >> trackerPtr->isQuickClick;
		fs["horSensitivity"] >> trackerPtr->horSensitivity;
		fs["verSensitivity"] >> trackerPtr->verSensitivity;
		fs["smilingTriggerAngle"] >> trackerPtr->smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> trackerPtr->minNeighborsSmileDetector;
		fs["dwellDisp"] >> mMouseDlg->dwellDisp;
		fs["dwellDuration"] >> mMouseDlg->dwellDuration;
		fs["secQuickClick"] >> mMouseDlg->secQuickClick;
		fs["secSmile"] >> mMouseDlg->secSmile;
		fs["secOneClick"] >> mMouseDlg->secOneClick;
		fs["secDoubleClick"] >> mMouseDlg->secDoubleClick;
		fs["secDwellStartTime"] >> mMouseDlg->secDwellStartTime;;
		fs["flipCameraFlag"] >> trackerPtr->flipCameraFlag;
		fs["showVideoFlag"] >> trackerPtr->showVideoFlag;
		fs["ewmaAlpha"] >> trackerPtr->ewmaAlpha;
//		fs["accumMoveRatio"] >> trackerPtr->accumMoveRatio;
		fs["velocitySlider"] >> trackerPtr->velocitySlider;
		fs["minFaceNeighbors"] >> trackerPtr->minFaceNeighbors;
		fs["editKeyboard"] >> ek; trackerPtr->editKeyboard = ek.c_str(); mOptionsDlg.back_mEditKeyboard = trackerPtr->editKeyboard;
		fs["needEqualize"] >> trackerPtr->needEqualize;
		fs["minDistRatio"] >> trackerPtr->minDistRatio;
		fs["minCornersCount"] >> trackerPtr->minCornersCount;
		fs["faceFrames"] >> faceFrames;
		fs["pauseTime"] >> pauseTime;
		fs["needSound"] >> trackerPtr->mMouseDlg->needSound;
		fs["enableMultithreading"] >> enableMultithreading;
	}
	fs.release();
}

void CFaceControllerMFCDlg::initSettings()
{
	changeLanguage(langNum);
	mOptionsDlg.pCAdvancedTab.mComboLanguage.SetCurSel(langNum);

	mOptionsDlg.pCMainTab.mSliderSpeedHorisontal.SetPos(trackerPtr->horSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSpeedVertical.SetPos(trackerPtr->verSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSmileAngle.SetPos(trackerPtr->smilingTriggerAngle);
	mOptionsDlg.pCMainTab.mSliderSmileSensitivity.SetPos(trackerPtr->minNeighborsSmileDetector);
	mOptionsDlg.pCMainTab.mSliderDwellDispl.SetPos(mMouseDlg->dwellDisp * 100);
	mOptionsDlg.pCMainTab.mSliderDwellTime.SetPos(mMouseDlg->dwellDuration * 10);
	mOptionsDlg.pCMainTab.mSliderQuickTime.SetPos(mMouseDlg->secQuickClick * 10);
	mOptionsDlg.pCMainTab.mSlider1x.SetPos(mMouseDlg->secSmile * 10);
	mOptionsDlg.pCMainTab.mSlider2x.SetPos(mMouseDlg->secOneClick * 10);
	mOptionsDlg.pCMainTab.mSliderCancel.SetPos(mMouseDlg->secDoubleClick * 10);
	mOptionsDlg.pCMainTab.mSliderTimeToDwell.SetPos(mMouseDlg->secDwellStartTime * 10);
	mOptionsDlg.pCAdvancedTab.mCheckFlipCamera.SetCheck(trackerPtr->flipCameraFlag);
	mOptionsDlg.pCAdvancedTab.mCheckEqualize.SetCheck(trackerPtr->needEqualize);
	mOptionsDlg.pCAdvancedTab.mCheckThreading.SetCheck(enableMultithreading);
	mOptionsDlg.pCAdvancedTab.mSliderAcceleration.SetPos(trackerPtr->velocitySlider * 10);
	mOptionsDlg.pCAdvancedTab.mSliderFace.SetPos(trackerPtr->minFaceNeighbors);
	mOptionsDlg.pCAdvancedTab.mSliderFaceMinNum.SetPos(trackerPtr->minCornersCount);
	mOptionsDlg.pCAdvancedTab.mSliderFaceFrames.SetPos(faceFrames + 1);
	mOptionsDlg.pCAdvancedTab.mSliderCustom.SetPos(trackerPtr->minDistRatio);
	mOptionsDlg.pCAdvancedTab.mSliderEWMA.SetPos(trackerPtr->ewmaAlpha * 100);
	mOptionsDlg.pCAdvancedTab.mSliderOnInput.SetPos(pauseTime * 10);
	mOptionsDlg.pCAdvancedTab.mCheckSound.SetCheck(trackerPtr->mMouseDlg->needSound);


	mOptionsDlg.pCAdvancedTab.flipCameraFlag = trackerPtr->flipCameraFlag;
	mOptionsDlg.pCAdvancedTab.needEqualize = trackerPtr->needEqualize;
	mOptionsDlg.pCAdvancedTab.needAutostart = needAutostart;
	mOptionsDlg.pCAdvancedTab.needSound = trackerPtr->mMouseDlg->needSound;
	mOptionsDlg.pCMainTab.fSliderSpeedHorisontal = trackerPtr->horSensitivity;
	mOptionsDlg.pCMainTab.fSliderSpeedVertical = trackerPtr->verSensitivity;
	mOptionsDlg.pCMainTab.iSliderSmileAngle = trackerPtr->smilingTriggerAngle;
	mOptionsDlg.pCMainTab.iSliderSmileSensitivity = trackerPtr->minNeighborsSmileDetector;
	mOptionsDlg.pCMainTab.fSliderDwellDispl = mMouseDlg->dwellDisp;
	mOptionsDlg.pCMainTab.fSliderDwellTime = mMouseDlg->dwellDuration;
	mOptionsDlg.pCMainTab.fSliderQuickTime = mMouseDlg->secQuickClick;
	mOptionsDlg.pCMainTab.fSlider1x = mMouseDlg->secSmile;
	mOptionsDlg.pCMainTab.fSlider2x = mMouseDlg->secOneClick;
	mOptionsDlg.pCMainTab.fSliderCancel = mMouseDlg->secDoubleClick;
	mOptionsDlg.pCMainTab.fSliderTimeToDwell = mMouseDlg->secDwellStartTime;
	mOptionsDlg.pCAdvancedTab.fSliderOnInput = pauseTime;
	mOptionsDlg.pCAdvancedTab.iSliderFaceFrames = faceFrames - 1;
	mOptionsDlg.pCAdvancedTab.fSliderAcceleration = trackerPtr->velocitySlider;
	trackerPtr->velocityK = trackerPtr->velocitySlider * 0.002 - 0.0014;
	mOptionsDlg.pCAdvancedTab.fSliderOnInput = pauseTime;
	mOptionsDlg.pCAdvancedTab.iSliderFace = trackerPtr->minFaceNeighbors;
	mOptionsDlg.pCAdvancedTab.mEditKeyboard = trackerPtr->editKeyboard;
	mOptionsDlg.pCAdvancedTab.fSliderEWMA = trackerPtr->ewmaAlpha;

	// save for cancel
	onApplyActions();


	mOptionsDlg.pCMainTab.UpdateData(FALSE);
	mOptionsDlg.pCAdvancedTab.UpdateData(FALSE);
}



void CFaceControllerMFCDlg::OnBnClickedButtonOcr()
{

	if (trackerPtr->editKeyboard.GetLength() < 1)
	{
		MessageBox(_T("Please add the path to the screen app in the settings!"));
		return;
	}

	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);
	//ShellExecute(this->GetSafeHwnd(), L"open", L"osk.exe", NULL, NULL, SW_SHOWNORMAL);OptikeyPro.exe

	
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = _T("open");                // Operation to perform
	shExInfo.lpFile = (LPCTSTR)trackerPtr->editKeyboard;       // Application to start    
	shExInfo.lpParameters = (LPCTSTR)"";                  // Additional parameters
	shExInfo.lpDirectory = 0;
	shExInfo.nShow = SW_SHOW;
	shExInfo.hInstApp = 0;

	if (ShellExecuteEx(&shExInfo) == false)
	{
		MessageBox(_T("Can't open onscreen keyboard. Installation of Optikey Pro or Click2Speak is recommended."));
		ShellExecute(this->GetSafeHwnd(), L"open", L"https://www.click2speak.net/download-sw/", NULL, NULL, SW_SHOWNORMAL);
		ShellExecute(this->GetSafeHwnd(), L"open", L"http://www.optikey.org/applications/optikey-pro", NULL, NULL, SW_SHOWNORMAL);
	};

	//	WaitForSingleObject(shExInfo.hProcess, INFINITE);
	//CloseHandle(shExInfo.hProcess);
}


void CFaceControllerMFCDlg::OnBnClickedButtonDrag()
{
	if (!isAutostart) force1Click();
	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	pButton_Drag->SetBitmap(bmp_dragp);
	trackerPtr->mMouseDlg->mouseClick = trackerPtr->mMouseDlg->DRAG;
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
	// TODO: Add your control notification handler code here
}


void CFaceControllerMFCDlg::OnNcMouseLeave()
{
	trackerPtr->mMouseDlg->isNonMainClientArea = false;
	CDialogEx::OnNcMouseLeave();

}


void CFaceControllerMFCDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{


	trackerPtr->mMouseDlg->isNonMainClientArea = true;


	CDialogEx::OnNcMouseMove(nHitTest, point);
}


void CFaceControllerMFCDlg::OnBnClickedButtonHide()
{
	if (trackerPtr->showVideoFlag)
	{
		trackerPtr->showVideoFlag = false;
		pButton_show->SetBitmap(bmp_showp);
		minimizeW();
	}
	else
	{
		trackerPtr->showVideoFlag = true;
		pButton_show->SetBitmap(bmp_show);
		maximizeW();
	}
	
}


void CFaceControllerMFCDlg::minimizeW()
{
	CRect wRect;
	GetWindowRect(wRect);
	MoveWindow(wRect.left, wRect.top, minWidth, minHeight, true);
	trackerPtr->showVideoFlag = false;
}

void CFaceControllerMFCDlg::maximizeW()
{
	CRect wRect;
	GetWindowRect(wRect);
	MoveWindow(wRect.left, wRect.top, maxRect.Width(), maxRect.Height(), true);
	trackerPtr->showVideoFlag = true;
	resizeCVWindow();

}


std::string CFaceControllerMFCDlg::ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

	std::wstring strtmp = std::wstring(buffer).substr(0, pos);
	std::string str(strtmp.begin(), strtmp.end());
	std::replace(str.begin(), str.end(), '\\', '/');
	str = str + "/";

	return str;
}

void CFaceControllerMFCDlg::OnBnClickedButtonChrome()
{

	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);
	ShellExecute(this->GetSafeHwnd(), L"open", L"https://preability.com/face-controlled-mouse/", NULL, NULL, SW_SHOWNORMAL);

}


LPWSTR CFaceControllerMFCDlg::ConvertString(const std::string& instr)
{
	// Assumes std::string is encoded in the current Windows ANSI codepage
	int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);

	if (bufferlen == 0)
	{
		// Something went wrong. Perhaps, check GetLastError() and log.
		return 0;
	}

	// Allocate new LPWSTR - must deallocate it later
	LPWSTR widestr = new WCHAR[bufferlen + 1];

	::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);

	// Ensure wide string is null terminated
	widestr[bufferlen] = 0;

	// Do something with widestr
	return widestr;
	//delete[] widestr;
}

void CFaceControllerMFCDlg::changeLanguage(int _langNum)
{
	CString str2;
	std::string strStd = std::to_string(_langNum);
	std::string istr;

		cv::FileStorage fs(sPathToFolder + "languages.xml", cv::FileStorage::READ);
		if (!fs.isOpened())
		{
			MessageBox(NULL, _T("Failed to open languages.xml file for reading!"), NULL);
			return;
		}
		else
		{
			if(needInitLang)
			{

				fs["languagesNum"] >> languagesNum;

				for (int i = 0; i <= languagesNum; i++)
				{
					istr = std::to_string(i);
					fs["statictext" + istr] >> statictextLanguage;
					str2 = statictextLanguage.c_str();
					mOptionsDlg.pCAdvancedTab.mComboLanguage.AddString(str2);
				}
			 }
			fs["statictext" + strStd + "0"] >> statictext0;
			fs["statictext" + strStd + "1"] >> statictext1;
			fs["statictext" + strStd + "2"] >> statictext2;
			fs["statictext" + strStd + "3"] >> statictext3;
			fs["statictext" + strStd + "4"] >> statictext4;
			fs["statictext" + strStd + "5"] >> statictext5;
			fs["statictext" + strStd + "6"] >> statictext6;
			fs["statictext" + strStd + "7"] >> statictext7;
			fs["statictext" + strStd + "8"] >> statictext8;
			fs["statictext" + strStd + "9"] >> statictext9;
			fs["statictext" + strStd + "10"] >> statictext10;
			fs["statictext" + strStd + "11"] >> statictext11;
			fs["statictext" + strStd + "12"] >> statictext12;
			fs["statictext" + strStd + "13"] >> statictext13;
			fs["statictext" + strStd + "14"] >> statictext14;
			fs["statictext" + strStd + "15"] >> statictext15;
			fs["statictext" + strStd + "16"] >> statictext16;
			fs["statictext" + strStd + "17"] >> statictext17;
			fs["statictext" + strStd + "18"] >> statictext18;
			fs["statictext" + strStd + "19"] >> statictext19;
			fs["statictext" + strStd + "20"] >> statictext20;
			fs["statictext" + strStd + "21"] >> statictext21;
			fs["statictext" + strStd + "22"] >> statictext22;
			fs["statictext" + strStd + "23"] >> statictext23;
			fs["statictext" + strStd + "24"] >> statictext24;
			fs["statictext" + strStd + "25"] >> statictext25;
			fs["statictext" + strStd + "26"] >> statictext26;
			fs["statictext" + strStd + "27"] >> statictext27;
			fs["statictext" + strStd + "28"] >> statictext28;
			fs["statictext" + strStd + "29"] >> statictext29;
			fs["statictext" + strStd + "30"] >> statictext30;
			fs["statictext" + strStd + "31"] >> statictext31;
			fs["statictext" + strStd + "32"] >> statictext32;
		}
		fs.release();
		if (needInitLang || _langNum!= langNum)
		{
			mOptionsDlg.pCMainTab.statictext4 = statictext4.c_str();
			mOptionsDlg.pCMainTab.statictext5 = statictext5.c_str();
			mOptionsDlg.pCMainTab.statictext6 = statictext6.c_str();
			mOptionsDlg.pCMainTab.statictext7 = statictext7.c_str();
			mOptionsDlg.pCMainTab.statictext8 = statictext8.c_str();
			mOptionsDlg.pCMainTab.statictext10 = statictext10.c_str();
			mOptionsDlg.pCMainTab.statictext11 = statictext11.c_str();
			mOptionsDlg.pCMainTab.statictext12 = statictext12.c_str();
			mOptionsDlg.pCMainTab.statictext14 = statictext14.c_str();
			mOptionsDlg.pCMainTab.statictext15 = statictext15.c_str();
			mOptionsDlg.pCMainTab.statictext16 = statictext16.c_str();

			mOptionsDlg.pCAdvancedTab.statictext22 = statictext22.c_str();
			mOptionsDlg.pCAdvancedTab.statictext23 = statictext23.c_str();
			mOptionsDlg.pCAdvancedTab.statictext25 = statictext25.c_str();
			mOptionsDlg.pCAdvancedTab.statictext27 = statictext27.c_str();
			mOptionsDlg.pCAdvancedTab.statictext28 = statictext28.c_str();
			mOptionsDlg.pCAdvancedTab.statictext29 = statictext29.c_str();
			mOptionsDlg.pCAdvancedTab.statictext30 = statictext30.c_str();


			mOptionsDlg.pCMainTab.GetDlgItem(IDC_STATIC_MOUSESPEED)->SetWindowText(ConvertString(statictext3));
			mOptionsDlg.pCMainTab.GetDlgItem(IDC_STATIC_LONGCLICK)->SetWindowText(ConvertString(statictext9));
			mOptionsDlg.pCMainTab.GetDlgItem(IDC_STATIC_DWELL)->SetWindowText(ConvertString(statictext13));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_CHECK_FLIPCAMERA)->SetWindowText(ConvertString(statictext17));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_CHECK_SOUND)->SetWindowText(ConvertString(statictext18));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_CHECK_EQUALIZE)->SetWindowText(ConvertString(statictext19));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(ConvertString(statictext20));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_STATIC_LANGUAGES)->SetWindowText(ConvertString(statictext21));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_STATIC_FACEREC)->SetWindowText(ConvertString(statictext26));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_STATIC_PATH)->SetWindowText(ConvertString(statictext31));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_BUTTON_RESET)->SetWindowText(ConvertString(statictext32));
			mOptionsDlg.GetDlgItem(IDC_APPLY)->SetWindowText(ConvertString(statictext1));
			mOptionsDlg.GetDlgItem(IDCANCEL)->SetWindowText(ConvertString(statictext2));
			mOptionsDlg.pCAdvancedTab.GetDlgItem(IDC_CHECK_THREADING)->SetWindowText(ConvertString(statictext24));

			mOptionsDlg.pCAdvancedTab.RedrawWindow();
			mOptionsDlg.RedrawWindow();
		}

		needInitLang = false;

}


