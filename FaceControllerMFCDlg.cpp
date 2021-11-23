
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
	ON_WM_MOUSEMOVE()
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

	pBigLogoPic->ModifyStyle(0, BS_BITMAP);

	///////////////////

	if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
	else if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
	else if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->RIGHT_CL)OnBnClickedButtonClickRight();

	if (tracker.detectDwellFlag)
	{
		pButton_dwell->SetBitmap(bmp_dwellp);
		pButton_ai->SetBitmap(bmp_ai);
		pButton_geo->SetBitmap(bmp_geo);
	}
	else
	{
		if (tracker.detectSmileGeoFlag) 
		{ 
			pButton_geo->SetBitmap(bmp_geop);
			pButton_dwell->SetBitmap(bmp_dwell);
		}
		if (tracker.detectSmileAIFlag) 
		{ 
			pButton_ai->SetBitmap(bmp_aip);
			pButton_dwell->SetBitmap(bmp_dwell);
		}
	}



	if (tracker.isQuickClick)pButton_timing->SetBitmap(bmp_quickclick);
	else pButton_timing->SetBitmap(bmp_timerclick);


	///////////////////
	pButton_off->SetBitmap(bmp_off);
	pButton_settings->SetBitmap(bmp_settings);
	pButton_start->SetBitmap(bmp_play);
	pButton_ABC->SetBitmap(bmp_ABC);
	pButton_Drag->SetBitmap(bmp_drag);

	pBigLogoPic->SetBitmap(bmp_bigLogo);



	bmp_bigLogo.SetBitmapDimension(clientPosition.Width(), clientPosition.Height() - 80);
	pBigLogoPic->MoveWindow(clientPosition.left, clientPosition.top, clientPosition.Width(), clientPosition.Height() - 80, true);

	mCheckAutostart.SetCheck(TRUE);

	

	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{

		m_ToolTip.AddTool(&mCheckAutostart, _T("Uncheck to abort tracking autostart"));
		m_ToolTip.AddTool(pButton_click1x, _T("One click mode"));
		m_ToolTip.AddTool(pButton_click2x, _T("Double click mode"));
		m_ToolTip.AddTool(pButton_clickRight, _T("Right click mode"));
		m_ToolTip.AddTool(pButton_geo, _T("Smile angle trigger for click"));
		m_ToolTip.AddTool(pButton_ai, _T("Smile AI trigger for click"));
		m_ToolTip.AddTool(pButton_dwell, _T("Turn on dwell mode for click"));
		m_ToolTip.AddTool(pButton_timing, _T("Switch timer or quick click"));
		m_ToolTip.AddTool(pButton_ABC, _T("Open Windows On-Screen Keyboard"));

		m_ToolTip.Activate(TRUE);
	}

	std::string cascadeName = "haarcascade_frontalface_alt2.xml";
	tracker.loadedFrontalfaceModelFlag = tracker.frontalFaceDetector.load(cascadeName);
	if (!tracker.loadedFrontalfaceModelFlag)
	{
		MessageBox(_T("Please, add haarcascade_frontalface_alt2.xml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	cascadeName = "haarcascade_smile.xml";
	tracker.loadedSmileModelFlag = tracker.smileDetector.load(cascadeName);
	if (!tracker.loadedSmileModelFlag)
	{
		MessageBox(_T("Please, add haarcascade_smile.xml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	// Create an instance of Facemark
	tracker.facemark = cv::face::FacemarkLBF::create();

	// Load landmark detector
	tracker.facemark->loadModel("lbfmodel.yaml");

	tracker.loadedFacemarkModelFlag = !tracker.facemark->empty();
	if (!tracker.loadedFacemarkModelFlag)
	{
		MessageBox(_T("Please, add lbfmodel.yaml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	bool openFlag = true;

	if (isAutostart)
	{
		SetTimer(ID_TIMER_1, 10000, NULL);
		SetTimer(ID_TIMER_2, 1000, NULL);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CFaceControllerMFCDlg::OnPaint()
{

	if (tracker.showVideoFlag)
	{ 
		GetClientRect(clientPosition);
		pBigLogoPic->MoveWindow(clientPosition.left, clientPosition.top, clientPosition.Width(), clientPosition.Height() - 80, true);
	}
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


	if (!tracker.openFlag)
	{
		MessageBox(_T("Please add the required files to the folder"));
		return;
	}

	for (int i = 0; i < 2402; i++)
	{
		tracker.cap.open(i);
		if (tracker.cap.isOpened()) break;
		else tracker.cap.release();
	}
	if (!tracker.cap.isOpened())
	{
		tracker.cap.release();
		MessageBox(_T("Please connect your webcam"));

		return;
	}

	if (tracker.turnOnTracking)
	{
		pButton_start->SetBitmap(bmp_pause);
		tracker.turnOnTracking = false;
		if (!tracker.mMouseDlg->IsWindowVisible()) tracker.mMouseDlg->ShowWindow(SW_SHOW);
		tracker.mMouseDlg->click = tracker.mMouseDlg->LOSTFACE;
		tracker.mMouseDlg->mStaticInfoText = (L"\U0001F60E");// sunglass

	}
	else
	{
		pButton_start->SetBitmap(bmp_play);
		tracker.turnOnTracking = true;
		tracker.mMouseDlg->ShowWindow(SW_HIDE);
	}

	if (needTrackerInitiate)
	{
		needTrackerInitiate = false;		

		cv::namedWindow("view", cv::WINDOW_FREERATIO);

		HWND hChild = (HWND)cvGetWindowHandle("view");
		HWND hParent = ::GetParent(hChild);

		::SetParent(hChild, GetDlgItem(IDC_PICTURE)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);

		CRect bitmapPosition;
		GetDlgItem(IDC_PICTURE)->GetWindowRect(bitmapPosition);
		cv::resizeWindow("view", bitmapPosition.right - bitmapPosition.left, bitmapPosition.bottom - bitmapPosition.top);

		tracker.defaultThreadsNum = cv::getNumThreads();
		tracker.allowResize = true;
		tracker.startTracking();

	}

}


void CFaceControllerMFCDlg::OnBnClickedOk()
{
	serializeChanges();
	tracker.openFlag = false;
	tracker.cap.release();
	CDialogEx::OnOK();
}

void CFaceControllerMFCDlg::OnClose()
{
	serializeChanges();
	tracker.openFlag = false;
	tracker.cap.release();
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

	CWnd::GetDesktopWindow()->GetWindowRect(ScreenRect);

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
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{

	case UWM_CUSTOMRIGHTCLICK:
		if (tracker.mMouseDlg->prevRightClick == tracker.mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
		if (tracker.mMouseDlg->prevRightClick == tracker.mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
		return 0;


	case UWM_UPDATE_OPTIONS:
		switch (wParam)
		{
			case IDC_SLIDER_SPEED_HORIZONTAL:
				tracker.horSensitivity = mOptionsDlg.pCMainTab.iSliderSpeedHorisontal;
			return 0;

			case IDC_SLIDER_SPEED_VERTICAL:
				tracker.verSensitivity = mOptionsDlg.pCMainTab.iSliderSpeedVertical;
			return 0;

			case IDC_SLIDER_SMILE_ANGLE:
				tracker.smilingTriggerAngle = mOptionsDlg.pCMainTab.iSliderSmileAngle;
			return 0;

			case IDC_SLIDER_SMILE_SENSITIVITY:
				tracker.minNeighborsSmileDetector = mOptionsDlg.pCMainTab.iSliderSmileSensitivity;
				return 0;

			case IDC_SLIDER_DWELL_DISPL:
				tracker.dwellDisp = mOptionsDlg.pCMainTab.iSliderDwellDispl; 
				return 0;

			case IDC_SLIDER_DWELL_TIME:
				tracker.dwellDuration = mOptionsDlg.pCMainTab.iSliderDwellTime;
				return 0;

			case IDC_SLIDER_QUICK_TIME:
				tracker.secQuickClick = mOptionsDlg.pCMainTab.iSliderQuickTime;
				return 0;

			case IDC_SLIDER_1X:
				tracker.secSmile = mOptionsDlg.pCMainTab.iSlider1x;
				return 0;

			case IDC_SLIDER_2X: 
				tracker.secOneClick = mOptionsDlg.pCMainTab.iSlider2x;
				return 0;

			case IDC_SLIDER_CANCEL:
				tracker.secDoubleClick = mOptionsDlg.pCMainTab.iSliderCancel;
				return 0;

			case IDC_SLIDER_TIMETODWELL:
				tracker.secDwellStartTime = mOptionsDlg.pCMainTab.iSliderTimeToDwell;
				return 0;
			
			case IDC_CHECK_FLIPCAMERA:
				tracker.flipCameraFlag = mOptionsDlg.pCAdvancedTab.flipCameraFlag;
				return 0;

			case IDC_CHECK_SHOWVIDEO:
				tracker.showVideoFlag = mOptionsDlg.pCAdvancedTab.showVideoFlag;
				return 0;
				
			case IDC_CHECK_2THREADS:
				tracker.use2ThreadsFlag = mOptionsDlg.pCAdvancedTab.use2ThreadsFlag;
				return 0;

			case IDC_STATIC_THREADSNUM:
				tracker.curThreadsNum = mOptionsDlg.pCAdvancedTab.curThreadsNum;
				return 0;

			case IDC_SLIDER_EWMA:
				tracker.ewmaAlpha = mOptionsDlg.pCAdvancedTab.iSliderEWMA;
				if (tracker.ewmaXfilter) tracker.ewmaXfilter->alpha = tracker.ewmaAlpha;
				if (tracker.ewmaYfilter) tracker.ewmaYfilter->alpha = tracker.ewmaAlpha;
				return 0;

			case IDC_SLIDER_MARKS:
				tracker.accumMoveRatio = mOptionsDlg.pCAdvancedTab.iSliderMarks;
				return 0;

			case IDC_SLIDER_ACCELERATION:
				tracker.velocitySlider = mOptionsDlg.pCAdvancedTab.iSliderAcceleration;
				tracker.velocityK = tracker.velocitySlider * 0.002 - 0.0014;
				return 0;

			case IDC_SLIDER_FACE_SENSITIVITY:
				tracker.minFaceNeighbors = mOptionsDlg.pCAdvancedTab.iSliderFace;
				return 0;

			case IDC_CHECK_EWMA:
				tracker.turnOnEWMA = mOptionsDlg.pCAdvancedTab.EWMAFlag;
				return 0;

			case IDCANCEL:
				tracker.horSensitivity = mOptionsDlg.back_iSliderSpeedHorisontal;
				tracker.verSensitivity = mOptionsDlg.back_iSliderSpeedVertical;
				tracker.smilingTriggerAngle = mOptionsDlg.back_iSliderSmileAngle;
				tracker.minNeighborsSmileDetector = mOptionsDlg.back_iSliderSmileSensitivity;
				tracker.dwellDisp = mOptionsDlg.back_iSliderDwellDispl;
				tracker.dwellDuration = mOptionsDlg.back_iSliderDwellTime;
				tracker.secQuickClick = mOptionsDlg.back_iSliderQuickTime;
				tracker.secSmile = mOptionsDlg.back_iSlider1x;
				tracker.secOneClick = mOptionsDlg.back_iSlider2x;
				tracker.secDoubleClick = mOptionsDlg.back_iSliderCancel;
				tracker.secDwellStartTime = mOptionsDlg.back_iSliderTimeToDwell;

				tracker.flipCameraFlag = mOptionsDlg.back_flipCameraFlag;
				tracker.showVideoFlag = mOptionsDlg.back_showVideoFlag;
				tracker.use2ThreadsFlag = mOptionsDlg.back_use2ThreadsFlag;
				tracker.turnOnEWMA = mOptionsDlg.back_EWMAFlag;
				tracker.ewmaAlpha = mOptionsDlg.back_iSliderEWMA;
				tracker.accumMoveRatio = mOptionsDlg.back_iSliderMarks;
				tracker.velocitySlider = mOptionsDlg.back_iSliderAcceleration;
				tracker.minFaceNeighbors = mOptionsDlg.back_iSliderFace;
				tracker.curThreadsNum = mOptionsDlg.back_curThreadsNum;

				return 0;

			case IDC_APPLY:

				mOptionsDlg.back_iSliderSpeedHorisontal = tracker.horSensitivity;
				mOptionsDlg.back_iSliderSpeedVertical = tracker.verSensitivity;
				mOptionsDlg.back_iSliderSmileAngle = tracker.smilingTriggerAngle;
				mOptionsDlg.back_iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
				mOptionsDlg.back_iSliderDwellDispl = tracker.dwellDisp;
				mOptionsDlg.back_iSliderDwellTime = tracker.dwellDuration;
				mOptionsDlg.back_iSliderQuickTime = tracker.secQuickClick;
				mOptionsDlg.back_iSlider1x = tracker.secSmile;
				mOptionsDlg.back_iSlider2x = tracker.secOneClick;
				mOptionsDlg.back_iSliderCancel = tracker.secDoubleClick;
				mOptionsDlg.back_iSliderTimeToDwell = tracker.secDwellStartTime;

				mOptionsDlg.back_flipCameraFlag = tracker.flipCameraFlag;
				mOptionsDlg.back_showVideoFlag = tracker.showVideoFlag;
				mOptionsDlg.back_use2ThreadsFlag = tracker.use2ThreadsFlag;
				mOptionsDlg.back_EWMAFlag = tracker.turnOnEWMA;
				mOptionsDlg.back_iSliderEWMA = tracker.ewmaAlpha;
				mOptionsDlg.back_iSliderMarks = tracker.accumMoveRatio;
				mOptionsDlg.back_iSliderAcceleration = tracker.velocitySlider;
				mOptionsDlg.back_iSliderFace = tracker.minFaceNeighbors;
				mOptionsDlg.back_curThreadsNum = tracker.curThreadsNum;

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
				GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
				GetDlgItem(IDC_CHECK_AUTOSTART)->EnableWindow(0);
				OnBnClickedStart();
				KillTimer(ID_TIMER_1);
				KillTimer(ID_TIMER_2);

			}
			else
			{
				GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
				KillTimer(ID_TIMER_1);
				KillTimer(ID_TIMER_2);
			}
			isAutostart = false;

			return 0;

		case ID_TIMER_2:

			if (isAutostart)
			{
				autostartSeconds++;
				int elapsedSeconds = ID_TIMER_1 / 1000 - autostartSeconds;
				if (elapsedSeconds >= 0)
				{
					strAutostartSeconds.Format(_T("%d"), elapsedSeconds);
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start ") + strAutostartSeconds + _T(" s"));
				}
				else
				{
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
					pButton_start->SetBitmap(bmp_pause);
					//isAutostart = false;
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
			tracker.isTimer2Clicks = false;
			KillTimer(ID_TIMER_2CLICKS);

			return 0;

		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CFaceControllerMFCDlg::OnBnClickedCheckAutostart()
{
	if (!isAutostart) force1Click();

	UpdateData(TRUE);
	if (mCheckAutostart.GetCheck() == NULL)
	{
		GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
		KillTimer(ID_TIMER_1);
		KillTimer(ID_TIMER_2);
		GetDlgItem(IDC_CHECK_AUTOSTART)->EnableWindow(0);
		isAutostart = false;
	}
}

BOOL CFaceControllerMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}



void CFaceControllerMFCDlg::OnBnClickedButtonGeo()
{
	if (!isAutostart) force1Click();
	tracker.detectSmileGeoFlag = true;
	pButton_geo->SetBitmap(bmp_geop);
	if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
	pButton_dwell->SetBitmap(bmp_dwell);
	pButton_dwell->RedrawWindow();
	pButton_geo->RedrawWindow();
	isCheckedButtonGeo = !isCheckedButtonGeo;

}


void CFaceControllerMFCDlg::OnBnClickedButtonAi()
{
	if (!isAutostart) force1Click();
	tracker.detectSmileAIFlag = true;
	pButton_ai->SetBitmap(bmp_aip);
	if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
	pButton_dwell->SetBitmap(bmp_dwell);
	pButton_dwell->RedrawWindow();
	pButton_ai->RedrawWindow();
	isCheckedButtonAi = !isCheckedButtonAi;

}



void CFaceControllerMFCDlg::OnBnClickedButtonDwell()
{
	if (!isAutostart) force1Click();
	tracker.detectDwellFlag = true;
	pButton_dwell->SetBitmap(bmp_dwellp);
	if (tracker.detectSmileAIFlag) tracker.detectSmileAIFlag = false;
	if (tracker.detectSmileGeoFlag) tracker.detectSmileGeoFlag = false;
	pButton_ai->SetBitmap(bmp_ai);
	pButton_geo->SetBitmap(bmp_geo);
	pButton_dwell->RedrawWindow();
	pButton_ai->RedrawWindow();
	pButton_geo->RedrawWindow();
	isCheckedButtonDwell = !isCheckedButtonDwell;
}



void CFaceControllerMFCDlg::OnBnClickedButtonClick1x()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1xp);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->ONE_CL;
	tracker.secDuration = tracker.secToOneClickCancelDuration;
	tracker.mMouseDlg->prevRightClick = tracker.mMouseDlg->ONE_CL;

}


void CFaceControllerMFCDlg::OnBnClickedButtonClick2x()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2xp);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->DOUBLE_CL;
	tracker.secDuration = tracker.secToDoubleClickCancelDuration;
	tracker.mMouseDlg->prevRightClick = tracker.mMouseDlg->DOUBLE_CL;
}


void CFaceControllerMFCDlg::OnBnClickedButtonClickRight()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRightp);
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->RIGHT_CL;
	tracker.secDuration = tracker.secToOneClickCancelDuration;
}

void CFaceControllerMFCDlg::OnBnClickedButtonTiming()
{
	if (!isAutostart) force1Click();

	if (tracker.isQuickClick)
	{
		pButton_timing->SetBitmap(bmp_timerclick);
	}
	else
	{
		pButton_timing->SetBitmap(bmp_quickclick);
	}
	tracker.isQuickClick = !tracker.isQuickClick;
}

void CFaceControllerMFCDlg::force1Click()
{
	if (tracker.isQuickClick)
	{
		tracker.isTimer2Clicks = true;
		SetTimer(ID_TIMER_2CLICKS, 200, NULL);
	}
}


void CFaceControllerMFCDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	CRect picturePosition;

	pBigLogoPic->GetWindowRect(picturePosition);

	if (tracker.allowResize)
	{
		cv::resizeWindow("view", picturePosition.right - picturePosition.left, picturePosition.bottom - picturePosition.top);
	}
		
}

// save main dialog parameters
void CFaceControllerMFCDlg::serializeChanges()
{
	cv::FileStorage fs("config.xml", cv::FileStorage::WRITE);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open config.xml file, your changes are not saved!"), NULL);
	else
	{
		fs << "detectSmileGeoFlag" << tracker.detectSmileGeoFlag;
		fs << "detectSmileAIFlag" << tracker.detectSmileAIFlag;
		fs << "detectDwellFlag" << tracker.detectDwellFlag;
		fs << "mouseClick" << tracker.mMouseDlg->mouseClick;
		fs << "isQuickClick" << tracker.isQuickClick;
		fs << "horSensitivity" << tracker.horSensitivity;
		fs << "verSensitivity" << tracker.verSensitivity;
		fs << "smilingTriggerAngle" << tracker.smilingTriggerAngle;
		fs << "minNeighborsSmileDetector" << tracker.minNeighborsSmileDetector;
		fs << "dwellDisp" << tracker.dwellDisp;
		fs << "dwellDuration" << tracker.dwellDuration;
		fs << "secQuickClick" << tracker.secQuickClick;
		fs << "secSmile" << tracker.secSmile;
		fs << "secOneClick" << tracker.secOneClick;
		fs << "secDoubleClick" << tracker.secDoubleClick;
		fs << "secDwellStartTime" << tracker.secDwellStartTime;
		fs << "curThreadsNum" << tracker.curThreadsNum;
		fs << "flipCameraFlag" << tracker.flipCameraFlag;
		fs << "showVideoFlag" << tracker.showVideoFlag;
		fs << "use2ThreadsFlag" << tracker.use2ThreadsFlag;
		fs << "ewmaAlpha" << tracker.ewmaAlpha;
		fs << "accumMoveRatio" << tracker.accumMoveRatio;
		fs << "velocitySlider" << tracker.velocitySlider;
		fs << "minFaceNeighbors" << tracker.minFaceNeighbors;
		fs << "turnOnEWMA" << tracker.turnOnEWMA;
		
	}
	fs.release();
}


//load
void CFaceControllerMFCDlg::readSerialized()
{
	cv::FileStorage fs("config.xml", cv::FileStorage::READ);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open config.xml file, your changes are not saved!"), NULL);
	else
	{
		fs["detectSmileGeoFlag"] >> tracker.detectSmileGeoFlag;
		fs["detectSmileAIFlag"] >> tracker.detectSmileAIFlag;
		fs["detectDwellFlag"] >> tracker.detectDwellFlag;
		fs["mouseClick"] >> tracker.mMouseDlg->mouseClick;
		fs["isQuickClick"] >> tracker.isQuickClick;
		fs["horSensitivity"] >> tracker.horSensitivity;
		fs["verSensitivity"] >> tracker.verSensitivity;
		fs["smilingTriggerAngle"] >> tracker.smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> tracker.minNeighborsSmileDetector;
		fs["dwellDisp"] >> tracker.dwellDisp;
		fs["dwellDuration"] >> tracker.dwellDuration;
		fs["secQuickClick"] >> tracker.secQuickClick;
		fs["secSmile"] >> tracker.secSmile;
		fs["secOneClick"] >> tracker.secOneClick;
		fs["secDoubleClick"] >> tracker.secDoubleClick;
		fs["secDwellStartTime"] >> tracker.secDwellStartTime;
		fs["curThreadsNum"] >> tracker.curThreadsNum;
		fs["flipCameraFlag"] >> tracker.flipCameraFlag;
		fs["showVideoFlag"] >> tracker.showVideoFlag;
		fs["use2ThreadsFlag"] >> tracker.use2ThreadsFlag;
		fs["ewmaAlpha"] >> tracker.ewmaAlpha;
		fs["accumMoveRatio"] >> tracker.accumMoveRatio;
		fs["velocitySlider"] >> tracker.velocitySlider;
		fs["minFaceNeighbors"] >> tracker.minFaceNeighbors;
		fs["turnOnEWMA"] >> tracker.turnOnEWMA;

	}
	fs.release();
}



//reset
void CFaceControllerMFCDlg::resetDefaults()
{
	cv::FileStorage fs("default.xml", cv::FileStorage::READ);
	if (!fs.isOpened()) MessageBox(NULL, _T("Failed to open default.xml file, your changes are not saved!"), NULL);
	else
	{
		fs["horSensitivity"] >> tracker.horSensitivity;
		fs["verSensitivity"] >> tracker.verSensitivity;
		fs["smilingTriggerAngle"] >> tracker.smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> tracker.minNeighborsSmileDetector;
		fs["dwellDisp"] >> tracker.dwellDisp;
		fs["dwellDuration"] >> tracker.dwellDuration;
		fs["secQuickClick"] >> tracker.secQuickClick;
		fs["secSmile"] >> tracker.secSmile;
		fs["secOneClick"] >> tracker.secOneClick;
		fs["secDoubleClick"] >> tracker.secDoubleClick;
		fs["secDwellStartTime"] >> tracker.secDwellStartTime;
		fs["curThreadsNum"] >> tracker.curThreadsNum;
		fs["flipCameraFlag"] >> tracker.flipCameraFlag;
		fs["showVideoFlag"] >> tracker.showVideoFlag;
		fs["use2ThreadsFlag"] >> tracker.use2ThreadsFlag;
		fs["ewmaAlpha"] >> tracker.ewmaAlpha;
		fs["accumMoveRatio"] >> tracker.accumMoveRatio;
		fs["velocitySlider"] >> tracker.velocitySlider;
		fs["minFaceNeighbors"] >> tracker.minFaceNeighbors;
		fs["turnOnEWMA"] >> tracker.turnOnEWMA;

	}
	fs.release();
}

void CFaceControllerMFCDlg::initSettings()
{
	mOptionsDlg.pCMainTab.mSliderSpeedHorisontal.SetPos(tracker.horSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSpeedVertical.SetPos(tracker.verSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSmileAngle.SetPos(tracker.smilingTriggerAngle);
	mOptionsDlg.pCMainTab.mSliderSmileSensitivity.SetPos(tracker.minNeighborsSmileDetector);
	mOptionsDlg.pCMainTab.mSliderDwellDispl.SetPos(tracker.dwellDisp * 10);
	mOptionsDlg.pCMainTab.mSliderDwellTime.SetPos(tracker.dwellDuration * 10);
	mOptionsDlg.pCMainTab.mSliderQuickTime.SetPos(tracker.secQuickClick * 10);
	mOptionsDlg.pCMainTab.mSlider1x.SetPos(tracker.secSmile * 10);
	mOptionsDlg.pCMainTab.mSlider2x.SetPos(tracker.secOneClick * 10);
	mOptionsDlg.pCMainTab.mSliderCancel.SetPos(tracker.secDoubleClick * 10);
	mOptionsDlg.pCMainTab.mSliderTimeToDwell.SetPos(tracker.secDwellStartTime * 10);


	mOptionsDlg.pCAdvancedTab.flipCameraFlag = tracker.flipCameraFlag;
	mOptionsDlg.pCAdvancedTab.showVideoFlag = tracker.showVideoFlag;
	mOptionsDlg.pCAdvancedTab.use2ThreadsFlag = tracker.use2ThreadsFlag;


	mOptionsDlg.pCAdvancedTab.mCheckFlipCamera.SetCheck(tracker.flipCameraFlag);
	mOptionsDlg.pCAdvancedTab.mCheckShowVideo.SetCheck(tracker.showVideoFlag);
	mOptionsDlg.pCAdvancedTab.mCheck2Threads.SetCheck(tracker.use2ThreadsFlag);

	mOptionsDlg.pCAdvancedTab.mSliderMarks.SetPos(tracker.accumMoveRatio * 100);
	mOptionsDlg.pCAdvancedTab.mSliderAcceleration.SetPos(tracker.velocitySlider * 10);
	mOptionsDlg.pCAdvancedTab.mSliderFace.SetPos(tracker.minFaceNeighbors);


	// Threads
	tracker.defaultThreadsNum = cv::getNumThreads();
	if (tracker.defaultThreadsNum < 1)tracker.defaultThreadsNum = 2;
	if (tracker.curThreadsNum > tracker.defaultThreadsNum)tracker.curThreadsNum = tracker.defaultThreadsNum;

	mOptionsDlg.pCAdvancedTab.defaultThreadsNum = tracker.defaultThreadsNum;
	mOptionsDlg.pCAdvancedTab.curThreadsNum = tracker.curThreadsNum;

	mOptionsDlg.pCAdvancedTab.mSlider2Threads.SetRange(1, tracker.defaultThreadsNum);
	mOptionsDlg.pCAdvancedTab.mSlider2Threads.SetPos(tracker.curThreadsNum);

	if (!tracker.use2ThreadsFlag) mOptionsDlg.pCAdvancedTab.mSlider2Threads.EnableWindow(0);

	// EWMA
	mOptionsDlg.pCAdvancedTab.EWMAFlag = tracker.turnOnEWMA;
	mOptionsDlg.pCAdvancedTab.mCheckEWMA.SetCheck(tracker.turnOnEWMA);
	mOptionsDlg.pCAdvancedTab.mSliderEWMA.SetPos(tracker.ewmaAlpha * 10);
	mOptionsDlg.pCAdvancedTab.iSliderEWMA = tracker.ewmaAlpha;
	if (!tracker.turnOnEWMA) mOptionsDlg.pCAdvancedTab.mSliderEWMA.EnableWindow(0);
	///////////


	mOptionsDlg.pCMainTab.iSliderSpeedHorisontal = tracker.horSensitivity;
	mOptionsDlg.pCMainTab.iSliderSpeedVertical = tracker.verSensitivity;
	mOptionsDlg.pCMainTab.iSliderSmileAngle = tracker.smilingTriggerAngle;
	mOptionsDlg.pCMainTab.iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
	mOptionsDlg.pCMainTab.iSliderDwellDispl = tracker.dwellDisp;
	mOptionsDlg.pCMainTab.iSliderDwellTime = tracker.dwellDuration;
	mOptionsDlg.pCMainTab.iSliderQuickTime = tracker.secQuickClick;
	mOptionsDlg.pCMainTab.iSlider1x = tracker.secSmile;
	mOptionsDlg.pCMainTab.iSlider2x = tracker.secOneClick;
	mOptionsDlg.pCMainTab.iSliderCancel = tracker.secDoubleClick;
	mOptionsDlg.pCMainTab.iSliderTimeToDwell = tracker.secDwellStartTime;

	mOptionsDlg.pCAdvancedTab.iSliderMarks = tracker.accumMoveRatio;
	mOptionsDlg.pCAdvancedTab.iSliderAcceleration = tracker.velocitySlider;
	tracker.velocityK = tracker.velocitySlider * 0.002 - 0.0014;

	mOptionsDlg.pCAdvancedTab.iSliderFace = tracker.minFaceNeighbors;

	// save for cancel

	mOptionsDlg.back_iSliderSpeedHorisontal  = tracker.horSensitivity;
	mOptionsDlg.back_iSliderSpeedVertical = tracker.verSensitivity;
	mOptionsDlg.back_iSliderSmileAngle = tracker.smilingTriggerAngle;
	mOptionsDlg.back_iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
	mOptionsDlg.back_iSliderDwellDispl = tracker.dwellDisp;
	mOptionsDlg.back_iSliderDwellTime = tracker.dwellDuration;
	mOptionsDlg.back_iSliderQuickTime = tracker.secQuickClick;
	mOptionsDlg.back_iSlider1x = tracker.secSmile;
	mOptionsDlg.back_iSlider2x = tracker.secOneClick;
	mOptionsDlg.back_iSliderCancel = tracker.secDoubleClick;
	mOptionsDlg.back_iSliderTimeToDwell = tracker.secDwellStartTime;

	mOptionsDlg.back_flipCameraFlag = tracker.flipCameraFlag;
	mOptionsDlg.back_showVideoFlag = tracker.showVideoFlag;
	mOptionsDlg.back_use2ThreadsFlag = tracker.use2ThreadsFlag;
	mOptionsDlg.back_EWMAFlag = tracker.turnOnEWMA;
	mOptionsDlg.back_iSliderEWMA = tracker.ewmaAlpha;
	mOptionsDlg.back_iSliderMarks = tracker.accumMoveRatio;
	mOptionsDlg.back_iSliderAcceleration = tracker.velocitySlider;
	mOptionsDlg.back_iSliderFace = tracker.minFaceNeighbors;
	mOptionsDlg.back_defaultThreadsNum = tracker.defaultThreadsNum;
	mOptionsDlg.back_curThreadsNum = tracker.curThreadsNum;
	mOptionsDlg.back_minFaceNeighbors = tracker.minFaceNeighbors;



	mOptionsDlg.pCMainTab.UpdateData(FALSE);
	mOptionsDlg.pCAdvancedTab.UpdateData(FALSE);
}



void CFaceControllerMFCDlg::OnBnClickedButtonOcr()
{

	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);
	ShellExecute((HWND)NULL, L"open", L"osk.exe", NULL, NULL, SW_SHOWNORMAL);

}


void CFaceControllerMFCDlg::OnBnClickedButtonDrag()
{
	if (!isAutostart) force1Click();
	pButton_click1x->SetBitmap(bmp_click1x);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	pButton_Drag->SetBitmap(bmp_dragp);
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->DRAG;
	tracker.secDuration = tracker.secToOneClickCancelDuration;


	// TODO: Add your control notification handler code here
}




void CFaceControllerMFCDlg::OnNcMouseLeave()
{

	tracker.mMouseDlg->isNonClientArea = false;
	CDialogEx::OnNcMouseLeave();
}


void CFaceControllerMFCDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{

	tracker.mMouseDlg->isNonClientArea = true;
	CDialogEx::OnNcMouseMove(nHitTest, point);
}

