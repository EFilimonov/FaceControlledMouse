
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
	//sPathToFolder = "";

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

	if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
	else if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
	else if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->RIGHT_CL)OnBnClickedButtonClickRight();


	if (tracker.showVideoFlag)
		pButton_show->SetBitmap(bmp_show);
	else pButton_show->SetBitmap(bmp_showp);

	pButton_ai->SetBitmap(bmp_ai);
	pButton_geo->SetBitmap(bmp_geo);
	pButton_dwell->SetBitmap(bmp_dwell);

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

	if (!tracker.showVideoFlag) minimizeW();
	else maximizeW();
	//////////


	if (isAutostart)
	{
		SetTimer(ID_TIMER_1, 11000, NULL);
		SetTimer(ID_TIMER_2, 1000, NULL);
	}

	//Create the ToolTip control
	if (!m_ToolTip.Create(this))
	{
		TRACE0("Unable to create the ToolTip!");
	}
	else
	{
		/*	CButton* ;
	CButton* ;*/
		m_ToolTip.AddTool(&mCheckAutostart, _T("Uncheck to abort tracking autostart"));
		m_ToolTip.AddTool(pButton_click1x, _T("One click mode"));
		m_ToolTip.AddTool(pButton_click2x, _T("Double click mode"));
		m_ToolTip.AddTool(pButton_clickRight, _T("Right click mode"));
		m_ToolTip.AddTool(pButton_geo, _T("Smile angle trigger for click"));
		m_ToolTip.AddTool(pButton_ai, _T("Smile AI trigger for click"));
		m_ToolTip.AddTool(pButton_dwell, _T("Turn on dwell mode for click"));
		m_ToolTip.AddTool(pButton_timing, _T("Switch timer or quick click"));
		m_ToolTip.AddTool(pButton_ABC, _T("Open custom On-Screen application"));
		m_ToolTip.AddTool(pButton_chrome, _T("Help, manual, program web page"));
		m_ToolTip.AddTool(pButton_Drag, _T("Drag and drop"));
		m_ToolTip.AddTool(pButton_show, _T("Minimize to panel"));
		m_ToolTip.Activate(TRUE);
	}

	//std::string cascadeName = "haarcascade_frontalface_alt2.xml";
	std::string cascadeName = sPathToFolder + "haarcascade_frontalface_alt2.xml";



	tracker.loadedFrontalfaceModelFlag = tracker.frontalFaceDetector.load(cascadeName);
	if (!tracker.loadedFrontalfaceModelFlag)
	{
		MessageBox(_T("Please, add haarcascade_frontalface_alt2.xml file to the programm folder"));
		bool openFlag = false;
		return -1;
	}

	//cascadeName = "haarcascade_smile.xml";
	cascadeName = sPathToFolder + "haarcascade_smile.xml";
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
	//tracker.facemark->loadModel("lbfmodel.yaml");
	tracker.facemark->loadModel(sPathToFolder + "lbfmodel.yaml");
	tracker.loadedFacemarkModelFlag = !tracker.facemark->empty();
	if (!tracker.loadedFacemarkModelFlag)
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

	if (tracker.allowResize) 
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


	if (!tracker.openFlag)
	{
		MessageBox(_T("Please add the required files to the folder"));
		return;
	}


	if (!tracker.cap.isOpened())
	{ 
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
	}

	if (tracker.buttonStop)
	{
		tracker.buttonStop = false;
		pButton_start->SetBitmap(bmp_pause);
		tracker.turnOffClick = false;
		if (!tracker.mMouseDlg->IsWindowVisible()) tracker.mMouseDlg->ShowWindow(SW_SHOW);
		tracker.mMouseDlg->click = tracker.mMouseDlg->NEUTRAL;
		tracker.mMouseDlg->mStaticInfoText = (L"\U0001F60E");// sunglass

	}
	else
	{
		tracker.buttonStop = true;
		pButton_start->SetBitmap(bmp_play);
		tracker.turnOffClick = true;
		tracker.OnUnhookMouse();
		tracker.mMouseDlg->ShowWindow(SW_HIDE);
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

		aspectRatio = (float) tracker.cap.get(cv::CAP_PROP_FRAME_WIDTH) / (float) tracker.cap.get(cv::CAP_PROP_FRAME_HEIGHT);

		GetClientRect(clientPosition);
		newHeight = clientPosition.Height() - btHeight;
		newTop = clientPosition.top + btHeight;
		newWidth = newHeight * aspectRatio;
		newLeft = clientPosition.left + (clientPosition.Width() - newWidth) / 2;
		pBigLogoPic->MoveWindow(newLeft, newTop, newWidth, newHeight, true);

		cv::resizeWindow("view", newWidth, newHeight);

		tracker.allowResize = true;

		OnPaint();
	
		if (needTrackerInitiate) tracker.initiateTracker();

		needTrackerInitiate = false;

		////////////
		// main flow
		////////////

		for (;;)
		{
			mMouseDlg->moveMouseDlg(!tracker.mouseHookPause);

			if (!tracker.Tracking()) break;
		} 
		
		tracker.UnhookMouse();
	}

}


void CFaceControllerMFCDlg::OnBnClickedOk()
{
	serializeChanges();
	tracker.openFlag = false;
	tracker.cap.release();
	tracker.UnhookMouse();
	CDialogEx::OnOK();
}

void CFaceControllerMFCDlg::OnClose()
{
	serializeChanges();
	tracker.openFlag = false;
	tracker.cap.release();
	tracker.UnhookMouse();
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
		tracker.skipHook = true;
		return 0;

	case UWM_CUSTOMSTARTTRACK:
			startwnd->GetWindowRect(tracker.buttonStartPosition);
		return 0;

	case UWM_CUSTOMRIGHTCLICK:
		if (tracker.mMouseDlg->prevRightClick == tracker.mMouseDlg->DOUBLE_CL)OnBnClickedButtonClick2x();
		if (tracker.mMouseDlg->prevRightClick == tracker.mMouseDlg->ONE_CL)OnBnClickedButtonClick1x();
		return 0;

	case UWM_CUSTOMDRAGMAIN:


		GetWindowRect(&w);

		if (((w.top + tracker.correctedDY) > 1) && ((w.bottom + tracker.correctedDY) < tracker.vertLimit))
		{
			w.top += tracker.correctedDY;
			w.bottom += tracker.correctedDY;
		}
		if (((w.right + tracker.correctedDX) < tracker.horLimit) && ((w.left + tracker.correctedDX) > 1))
		{
			w.left += tracker.correctedDX;
			w.right += tracker.correctedDX;
		}
		MoveWindow(&w);
		OnPaint();


		return 0;


	case UWM_CUSTOMDRAGOPTION:


		mOptionsDlg.GetWindowRect(&w);

		if (((w.top + tracker.correctedDY) > 1) && ((w.bottom + tracker.correctedDY) < tracker.vertLimit))
		{
			w.top += tracker.correctedDY;
			w.bottom += tracker.correctedDY;
		}
		if (((w.right + tracker.correctedDX) < tracker.horLimit) && ((w.left + tracker.correctedDX) > 1))
		{
			w.left += tracker.correctedDX;
			w.right += tracker.correctedDX;
		}
		mOptionsDlg.MoveWindow(&w);
		mOptionsDlg.OnPaint();


		return 0;

	case UWM_UPDATE_OPTIONS:
		switch (wParam)
		{
		case IDC_CHECK_SOUND:
				mMouseDlg->needSound = mOptionsDlg.pCAdvancedTab.needSound;
			return 0;

			case IDC_SLIDER_ONINPUT:
				tracker.pauseTime = mOptionsDlg.pCAdvancedTab.fSliderOnInput;
				return 0;

			case IDC_CHECK_AUTOSTART:
				needAutostart = mOptionsDlg.pCAdvancedTab.needAutostart;
				return 0;

			case IDC_SLIDER_CUSTOM:
				tracker.minDistRatio = (double) mOptionsDlg.pCAdvancedTab.iSliderCustom;
				tracker.needToTrackerInit = true;
				return 0;

			case IDC_SLIDER_FACE_MINDIST:
				tracker.maxCornersCount = mOptionsDlg.pCAdvancedTab.iSliderMaxNum;
				tracker.needToTrackerInit = true;
			return 0;

			case IDC_SLIDER_FACE_MINNUM:
				tracker.minCornersCount = mOptionsDlg.pCAdvancedTab.iSliderMinNum;
				tracker.needToTrackerInit = true;
			return 0;

			case IDC_SLIDER_SPEED_HORIZONTAL:
				tracker.horSensitivity = mOptionsDlg.pCMainTab.fSliderSpeedHorisontal;
			return 0;

			case IDC_SLIDER_SPEED_VERTICAL:
				tracker.verSensitivity = mOptionsDlg.pCMainTab.fSliderSpeedVertical;
			return 0;

			case IDC_SLIDER_SMILE_ANGLE:
				tracker.smilingTriggerAngle = mOptionsDlg.pCMainTab.iSliderSmileAngle;
			return 0;

			case IDC_SLIDER_SMILE_SENSITIVITY:
				tracker.minNeighborsSmileDetector = mOptionsDlg.pCMainTab.iSliderSmileSensitivity;
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
				tracker.flipCameraFlag = mOptionsDlg.pCAdvancedTab.flipCameraFlag;
				return 0;
				
			case IDC_SLIDER_EWMA:
				tracker.ewmaAlpha = mOptionsDlg.pCAdvancedTab.fSliderEWMA;
				tracker.needToTrackerInit = true;
				return 0;

			case IDC_SLIDER_MARKS:
				tracker.accumMoveRatio = mOptionsDlg.pCAdvancedTab.fSliderMarks;
				tracker.needToTrackerInit = true;
				return 0;

			case IDC_SLIDER_ACCELERATION:
				tracker.velocitySlider = mOptionsDlg.pCAdvancedTab.fSliderAcceleration;
				tracker.velocityK = tracker.velocitySlider * 0.002 - 0.0014;
				return 0;

			case IDC_SLIDER_FACE_SENSITIVITY:
				tracker.minFaceNeighbors = mOptionsDlg.pCAdvancedTab.iSliderFace;
				return 0;

			case IDC_EDIT_KEYBOARD:
				tracker.editKeyboard = mOptionsDlg.pCAdvancedTab.mEditKeyboard;
				return 0;

			case IDC_CHECK_EQUALIZE:
				tracker.needEqualize = mOptionsDlg.pCAdvancedTab.needEqualize;
				return 0;

			case IDCANCEL:
				tracker.horSensitivity = mOptionsDlg.back_fSliderSpeedHorisontal;
				tracker.verSensitivity = mOptionsDlg.back_fSliderSpeedVertical;
				tracker.smilingTriggerAngle = mOptionsDlg.back_iSliderSmileAngle;
				tracker.minNeighborsSmileDetector = mOptionsDlg.back_iSliderSmileSensitivity;
				mMouseDlg->dwellDisp = mOptionsDlg.back_fSliderDwellDispl;
				mMouseDlg->dwellDuration = mOptionsDlg.back_fSliderDwellTime;
				mMouseDlg->secQuickClick = mOptionsDlg.back_fSliderQuickTime;
				mMouseDlg->secSmile = mOptionsDlg.back_fSlider1x;
				mMouseDlg->secOneClick = mOptionsDlg.back_fSlider2x;
				mMouseDlg->secDoubleClick = mOptionsDlg.back_fSliderCancel;
				mMouseDlg->secDwellStartTime = mOptionsDlg.back_fSliderTimeToDwell;
				tracker.mMouseDlg->needSound = mOptionsDlg.back_needSound;

				tracker.flipCameraFlag = mOptionsDlg.back_flipCameraFlag;
				tracker.needEqualize = mOptionsDlg.back_needEqualize;
				tracker.ewmaAlpha = mOptionsDlg.back_fSliderEWMA;
				tracker.accumMoveRatio = mOptionsDlg.back_fSliderMarks;
				tracker.velocitySlider = mOptionsDlg.back_fSliderAcceleration;
				tracker.minFaceNeighbors = mOptionsDlg.back_iSliderFace;
				tracker.editKeyboard = mOptionsDlg.back_mEditKeyboard;
				tracker.pauseTime = mOptionsDlg.back_fSliderOnInput;

				return 0;

			case IDC_APPLY:

				mOptionsDlg.back_fSliderSpeedHorisontal = tracker.horSensitivity;
				mOptionsDlg.back_fSliderSpeedVertical = tracker.verSensitivity;
				mOptionsDlg.back_iSliderSmileAngle = tracker.smilingTriggerAngle;
				mOptionsDlg.back_iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
				mOptionsDlg.back_fSliderDwellDispl = mMouseDlg->dwellDisp;
				mOptionsDlg.back_fSliderDwellTime = mMouseDlg->dwellDuration;
				mOptionsDlg.back_fSliderQuickTime = mMouseDlg->secQuickClick;
				mOptionsDlg.back_fSlider1x = mMouseDlg->secSmile;
				mOptionsDlg.back_fSlider2x = mMouseDlg->secOneClick;
				mOptionsDlg.back_fSliderCancel = mMouseDlg->secDoubleClick;
				mOptionsDlg.back_fSliderTimeToDwell = mMouseDlg->secDwellStartTime;
				mOptionsDlg.back_needSound = tracker.mMouseDlg->needSound;

				mOptionsDlg.back_flipCameraFlag = tracker.flipCameraFlag;
				mOptionsDlg.back_needEqualize = tracker.needEqualize;
				mOptionsDlg.back_fSliderEWMA = tracker.ewmaAlpha;
				mOptionsDlg.back_fSliderMarks = tracker.accumMoveRatio;
				mOptionsDlg.back_fSliderAcceleration = tracker.velocitySlider;
				mOptionsDlg.back_iSliderFace = tracker.minFaceNeighbors;
				mOptionsDlg.back_mEditKeyboard = tracker.editKeyboard;
				mOptionsDlg.back_fSliderOnInput = tracker.pauseTime;

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
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start ") + strAutostartSeconds + _T(" s"));
				}
				else
				{
					GetDlgItem(IDC_CHECK_AUTOSTART)->SetWindowText(_T("Start"));
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
			tracker.mMouseDlg->isTimer2Clicks = false;
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



	if (pMsg->hwnd == pButton_chrome->m_hWnd && pMsg->message == WM_MOUSEMOVE)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	}

	return CDialog::PreTranslateMessage(pMsg);
}



void CFaceControllerMFCDlg::OnBnClickedButtonGeo()
{
	if (!isAutostart) force1Click();

	if (tracker.detectSmileGeoFlag == true && tracker.detectSmileAIFlag == true)
	{
		tracker.detectSmileGeoFlag = false;
		pButton_geo->SetBitmap(bmp_geo);
		if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_geo->RedrawWindow();
	}
	else if (tracker.detectSmileGeoFlag == true && tracker.detectSmileAIFlag == true)
	{

	}
	else
	{ 
		tracker.detectSmileGeoFlag = true;
		pButton_geo->SetBitmap(bmp_geop);
		if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_geo->RedrawWindow();

	}
}


void CFaceControllerMFCDlg::OnBnClickedButtonAi()
{
	if (!isAutostart) force1Click();
	if (tracker.detectSmileGeoFlag == true && tracker.detectSmileAIFlag == true)
	{
		tracker.detectSmileAIFlag = false;
		pButton_ai->SetBitmap(bmp_ai);
		if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_ai->RedrawWindow();
	}
	else
	{
		tracker.detectSmileAIFlag = true;
		pButton_ai->SetBitmap(bmp_aip);
		if (tracker.detectDwellFlag)tracker.detectDwellFlag = false;
		pButton_dwell->SetBitmap(bmp_dwell);
		pButton_dwell->RedrawWindow();
		pButton_ai->RedrawWindow();
	}
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
}



void CFaceControllerMFCDlg::OnBnClickedButtonClick1x()
{
	if (!isAutostart) force1Click();

	pButton_click1x->SetBitmap(bmp_click1xp);
	pButton_click2x->SetBitmap(bmp_click2x);
	pButton_Drag->SetBitmap(bmp_drag);
	pButton_clickRight->SetBitmap(bmp_clickRight);
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->ONE_CL;
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
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
	mMouseDlg->secDuration = mMouseDlg->secToDoubleClickCancelDuration;
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
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
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
		tracker.mMouseDlg->isTimer2Clicks = true;
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
		fs << "detectSmileGeoFlag" << tracker.detectSmileGeoFlag;
		fs << "detectSmileAIFlag" << tracker.detectSmileAIFlag;
		fs << "detectDwellFlag" << tracker.detectDwellFlag;
		if (tracker.mMouseDlg->mouseClick == tracker.mMouseDlg->DRAG)tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->ONE_CL;
		fs << "mouseClick" << tracker.mMouseDlg->mouseClick;
		fs << "isQuickClick" << tracker.isQuickClick;
		fs << "horSensitivity" << tracker.horSensitivity;
		fs << "verSensitivity" << tracker.verSensitivity;
		fs << "smilingTriggerAngle" << tracker.smilingTriggerAngle;
		fs << "minNeighborsSmileDetector" << tracker.minNeighborsSmileDetector;
		fs << "dwellDisp" << mMouseDlg->dwellDisp;
		fs << "dwellDuration" << mMouseDlg->dwellDuration;
		fs << "secQuickClick" << mMouseDlg->secQuickClick;
		fs << "secSmile" << mMouseDlg->secSmile;
		fs << "secOneClick" << mMouseDlg->secOneClick;
		fs << "secDoubleClick" << mMouseDlg->secDoubleClick;
		fs << "secDwellStartTime" << mMouseDlg->secDwellStartTime;
		fs << "flipCameraFlag" << tracker.flipCameraFlag;
		fs << "showVideoFlag" << tracker.showVideoFlag;
		fs << "ewmaAlpha" << tracker.ewmaAlpha;
		fs << "accumMoveRatio" << tracker.accumMoveRatio;
		fs << "velocitySlider" << tracker.velocitySlider;
		fs << "minFaceNeighbors" << tracker.minFaceNeighbors;
		std::string strStd = CT2A(tracker.editKeyboard);
		ek = strStd;
		fs << "editKeyboard" << ek;
		fs << "needEqualize" << tracker.needEqualize;
		fs << "needAutostart" << needAutostart;
		fs << "minDistRatio" << tracker.minDistRatio;
		fs << "minCornersCount" << tracker.minCornersCount;
		fs << "maxCornersCount" << tracker.maxCornersCount;
		fs << "pauseTime" << tracker.pauseTime;
		fs << "needSound" << tracker.mMouseDlg->needSound;
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
		fs["detectSmileGeoFlag"] >> tracker.detectSmileGeoFlag;
		fs["detectSmileAIFlag"] >> tracker.detectSmileAIFlag;
		fs["detectDwellFlag"] >> tracker.detectDwellFlag;
		fs["mouseClick"] >> tracker.mMouseDlg->mouseClick;
		fs["isQuickClick"] >> tracker.isQuickClick;
		fs["horSensitivity"] >> tracker.horSensitivity;
		fs["verSensitivity"] >> tracker.verSensitivity;
		fs["smilingTriggerAngle"] >> tracker.smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> tracker.minNeighborsSmileDetector;
		fs["dwellDisp"] >> mMouseDlg->dwellDisp;
		fs["dwellDuration"] >> mMouseDlg->dwellDuration;
		fs["secQuickClick"] >> mMouseDlg->secQuickClick;
		fs["secSmile"] >> mMouseDlg->secSmile;
		fs["secOneClick"] >> mMouseDlg->secOneClick;
		fs["secDoubleClick"] >> mMouseDlg->secDoubleClick;
		fs["secDwellStartTime"] >> mMouseDlg->secDwellStartTime;;
		fs["flipCameraFlag"] >> tracker.flipCameraFlag;
		fs["showVideoFlag"] >> tracker.showVideoFlag;
		fs["ewmaAlpha"] >> tracker.ewmaAlpha;
		fs["accumMoveRatio"] >> tracker.accumMoveRatio;
		fs["velocitySlider"] >> tracker.velocitySlider;
		fs["minFaceNeighbors"] >> tracker.minFaceNeighbors;
		fs["editKeyboard"] >> ek; tracker.editKeyboard= ek.c_str(); mOptionsDlg.back_mEditKeyboard = tracker.editKeyboard;
		fs["needEqualize"] >>  tracker.needEqualize;
		fs["needAutostart"] >> needAutostart;
		fs["minDistRatio"] >> tracker.minDistRatio;
		fs["minCornersCount"] >> tracker.minCornersCount;
		fs["maxCornersCount"] >> tracker.maxCornersCount;
		fs["pauseTime"] >> tracker.pauseTime;
		fs["needSound"] >> tracker.mMouseDlg->needSound;
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
		fs["detectSmileGeoFlag"] >> tracker.detectSmileGeoFlag;
		fs["detectSmileAIFlag"] >> tracker.detectSmileAIFlag;
		fs["detectDwellFlag"] >> tracker.detectDwellFlag;
		fs["mouseClick"] >> tracker.mMouseDlg->mouseClick;
		fs["isQuickClick"] >> tracker.isQuickClick;
		fs["horSensitivity"] >> tracker.horSensitivity;
		fs["verSensitivity"] >> tracker.verSensitivity;
		fs["smilingTriggerAngle"] >> tracker.smilingTriggerAngle;
		fs["minNeighborsSmileDetector"] >> tracker.minNeighborsSmileDetector;
		fs["dwellDisp"] >> mMouseDlg->dwellDisp;
		fs["dwellDuration"] >> mMouseDlg->dwellDuration;
		fs["secQuickClick"] >> mMouseDlg->secQuickClick;
		fs["secSmile"] >> mMouseDlg->secSmile;
		fs["secOneClick"] >> mMouseDlg->secOneClick;
		fs["secDoubleClick"] >> mMouseDlg->secDoubleClick;
		fs["secDwellStartTime"] >> mMouseDlg->secDwellStartTime;;
		fs["flipCameraFlag"] >> tracker.flipCameraFlag;
		fs["showVideoFlag"] >> tracker.showVideoFlag;
		fs["ewmaAlpha"] >> tracker.ewmaAlpha;
		fs["accumMoveRatio"] >> tracker.accumMoveRatio;
		fs["velocitySlider"] >> tracker.velocitySlider;
		fs["minFaceNeighbors"] >> tracker.minFaceNeighbors;
		fs["editKeyboard"] >> ek; tracker.editKeyboard = ek.c_str(); mOptionsDlg.back_mEditKeyboard = tracker.editKeyboard;
		fs["needEqualize"] >> tracker.needEqualize;
		fs["minDistRatio"] >> tracker.minDistRatio;
		fs["minCornersCount"] >> tracker.minCornersCount;
		fs["maxCornersCount"] >> tracker.maxCornersCount;
		fs["pauseTime"] >> tracker.pauseTime;
		fs["needSound"] >> tracker.mMouseDlg->needSound;
	}
	fs.release();
}

void CFaceControllerMFCDlg::initSettings()
{
	// initiate controls
	mOptionsDlg.pCMainTab.mSliderSpeedHorisontal.SetPos(tracker.horSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSpeedVertical.SetPos(tracker.verSensitivity * 10);
	mOptionsDlg.pCMainTab.mSliderSmileAngle.SetPos(tracker.smilingTriggerAngle);
	mOptionsDlg.pCMainTab.mSliderSmileSensitivity.SetPos(tracker.minNeighborsSmileDetector);
	mOptionsDlg.pCMainTab.mSliderDwellDispl.SetPos(mMouseDlg->dwellDisp * 100);
	mOptionsDlg.pCMainTab.mSliderDwellTime.SetPos(mMouseDlg->dwellDuration * 10);
	mOptionsDlg.pCMainTab.mSliderQuickTime.SetPos(mMouseDlg->secQuickClick * 10);
	mOptionsDlg.pCMainTab.mSlider1x.SetPos(mMouseDlg->secSmile * 10);
	mOptionsDlg.pCMainTab.mSlider2x.SetPos(mMouseDlg->secOneClick * 10);
	mOptionsDlg.pCMainTab.mSliderCancel.SetPos(mMouseDlg->secDoubleClick * 10);
	mOptionsDlg.pCMainTab.mSliderTimeToDwell.SetPos(mMouseDlg->secDwellStartTime * 10);
	mOptionsDlg.pCAdvancedTab.mCheckFlipCamera.SetCheck(tracker.flipCameraFlag);
	mOptionsDlg.pCAdvancedTab.mCheckEqualize.SetCheck(tracker.needEqualize);
	mOptionsDlg.pCAdvancedTab.mSliderMarks.SetPos(tracker.accumMoveRatio * 100);
	mOptionsDlg.pCAdvancedTab.mSliderAcceleration.SetPos(tracker.velocitySlider * 10);
	mOptionsDlg.pCAdvancedTab.mSliderFace.SetPos(tracker.minFaceNeighbors);
	mOptionsDlg.pCAdvancedTab.mSliderFaceMinNum.SetPos(tracker.minCornersCount);
	mOptionsDlg.pCAdvancedTab.mSliderFaceMaxNum.SetPos(tracker.maxCornersCount);
	mOptionsDlg.pCAdvancedTab.mSliderCustom.SetPos(tracker.minDistRatio);
	mOptionsDlg.pCAdvancedTab.mSliderEWMA.SetPos(tracker.ewmaAlpha * 100);
	mOptionsDlg.pCAdvancedTab.mSliderOnInput.SetPos(tracker.pauseTime * 10);
	mOptionsDlg.pCAdvancedTab.mCheckSound.SetCheck(tracker.mMouseDlg->needSound);


	mOptionsDlg.pCAdvancedTab.flipCameraFlag = tracker.flipCameraFlag;
	mOptionsDlg.pCAdvancedTab.needEqualize = tracker.needEqualize;
	mOptionsDlg.pCAdvancedTab.needAutostart = needAutostart;
	mOptionsDlg.pCAdvancedTab.needSound = tracker.mMouseDlg->needSound;
	mOptionsDlg.pCMainTab.fSliderSpeedHorisontal = tracker.horSensitivity;
	mOptionsDlg.pCMainTab.fSliderSpeedVertical = tracker.verSensitivity;
	mOptionsDlg.pCMainTab.iSliderSmileAngle = tracker.smilingTriggerAngle;
	mOptionsDlg.pCMainTab.iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
	mOptionsDlg.pCMainTab.fSliderDwellDispl = mMouseDlg->dwellDisp;
	mOptionsDlg.pCMainTab.fSliderDwellTime = mMouseDlg->dwellDuration;
	mOptionsDlg.pCMainTab.fSliderQuickTime = mMouseDlg->secQuickClick;
	mOptionsDlg.pCMainTab.fSlider1x = mMouseDlg->secSmile;
	mOptionsDlg.pCMainTab.fSlider2x = mMouseDlg->secOneClick;
	mOptionsDlg.pCMainTab.fSliderCancel = mMouseDlg->secDoubleClick;
	mOptionsDlg.pCMainTab.fSliderTimeToDwell = mMouseDlg->secDwellStartTime;
	mOptionsDlg.pCAdvancedTab.fSliderOnInput = tracker.pauseTime;
	mOptionsDlg.pCAdvancedTab.fSliderMarks = tracker.accumMoveRatio;
	mOptionsDlg.pCAdvancedTab.fSliderAcceleration = tracker.velocitySlider;
	tracker.velocityK = tracker.velocitySlider * 0.002 - 0.0014;
	mOptionsDlg.pCAdvancedTab.fSliderOnInput = tracker.pauseTime;
	mOptionsDlg.pCAdvancedTab.iSliderFace = tracker.minFaceNeighbors;
	mOptionsDlg.pCAdvancedTab.mEditKeyboard = tracker.editKeyboard;
	mOptionsDlg.pCAdvancedTab.fSliderEWMA = tracker.ewmaAlpha;

	// save for cancel

	mOptionsDlg.back_fSliderSpeedHorisontal  = tracker.horSensitivity;
	mOptionsDlg.back_fSliderSpeedVertical = tracker.verSensitivity;
	mOptionsDlg.back_iSliderSmileAngle = tracker.smilingTriggerAngle;
	mOptionsDlg.back_iSliderSmileSensitivity = tracker.minNeighborsSmileDetector;
	mOptionsDlg.back_fSliderDwellDispl = mMouseDlg->dwellDisp;
	mOptionsDlg.back_fSliderDwellTime = mMouseDlg->dwellDuration;
	mOptionsDlg.back_fSliderQuickTime = mMouseDlg->secQuickClick;
	mOptionsDlg.back_fSlider1x = mMouseDlg->secSmile;
	mOptionsDlg.back_fSlider2x = mMouseDlg->secOneClick;
	mOptionsDlg.back_fSliderCancel = mMouseDlg->secDoubleClick;
	mOptionsDlg.back_fSliderTimeToDwell = mMouseDlg->secDwellStartTime;
	mOptionsDlg.back_fSliderOnInput = tracker.pauseTime;
	mOptionsDlg.back_needSound = tracker.mMouseDlg->needSound;

	mOptionsDlg.back_flipCameraFlag = tracker.flipCameraFlag;
	mOptionsDlg.back_needEqualize = tracker.needEqualize;
	mOptionsDlg.back_fSliderEWMA = tracker.ewmaAlpha;
	mOptionsDlg.back_fSliderMarks = tracker.accumMoveRatio;
	mOptionsDlg.back_fSliderAcceleration = tracker.velocitySlider;
	mOptionsDlg.back_iSliderFace = tracker.minFaceNeighbors;
	mOptionsDlg.back_minFaceNeighbors = tracker.minFaceNeighbors;
	mOptionsDlg.back_editKeyboard = tracker.editKeyboard;



	mOptionsDlg.pCMainTab.UpdateData(FALSE);
	mOptionsDlg.pCAdvancedTab.UpdateData(FALSE);
}



void CFaceControllerMFCDlg::OnBnClickedButtonOcr()
{

	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);
	//ShellExecute(this->GetSafeHwnd(), L"open", L"osk.exe", NULL, NULL, SW_SHOWNORMAL);OptikeyPro.exe

	
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = _T("open");                // Operation to perform
	shExInfo.lpFile = (LPCTSTR)tracker.editKeyboard;       // Application to start    
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
	tracker.mMouseDlg->mouseClick = tracker.mMouseDlg->DRAG;
	mMouseDlg->secDuration = mMouseDlg->secToOneClickCancelDuration;
	// TODO: Add your control notification handler code here
}


void CFaceControllerMFCDlg::OnNcMouseLeave()
{

	tracker.mMouseDlg->isNonMainClientArea = false;
	tracker.isNonMainClientArea = false;
	CDialogEx::OnNcMouseLeave();

}


void CFaceControllerMFCDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{


	tracker.mMouseDlg->isNonMainClientArea = true;
	tracker.isNonMainClientArea = true;


	CDialogEx::OnNcMouseMove(nHitTest, point);
}


void CFaceControllerMFCDlg::OnBnClickedButtonHide()
{
	if (tracker.showVideoFlag)
	{
		tracker.showVideoFlag = false;
		pButton_show->SetBitmap(bmp_showp);
		minimizeW();
	}
	else
	{
		tracker.showVideoFlag = true;
		pButton_show->SetBitmap(bmp_show);
		maximizeW();
	}
	
}


void CFaceControllerMFCDlg::minimizeW()
{
	CRect wRect;
	GetWindowRect(wRect);
	MoveWindow(wRect.left, wRect.top, minWidth, minHeight, true);
	tracker.showVideoFlag = false;
}

void CFaceControllerMFCDlg::maximizeW()
{
	CRect wRect;
	GetWindowRect(wRect);
	MoveWindow(wRect.left, wRect.top, maxRect.Width(), maxRect.Height(), true);
	tracker.showVideoFlag = true;
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



