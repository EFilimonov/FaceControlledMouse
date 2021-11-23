// CAdvancedTab.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "CAdvancedTab.h"
#include "afxdialogex.h"
#include <opencv2/core.hpp>


// CAdvancedTab dialog

IMPLEMENT_DYNAMIC(CAdvancedTab, CDialogEx)

CAdvancedTab::CAdvancedTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{

}

CAdvancedTab::~CAdvancedTab()
{
}

void CAdvancedTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_FLIPCAMERA, mCheckFlipCamera);
	DDX_Control(pDX, IDC_CHECK_SHOWVIDEO, mCheckShowVideo);
	DDX_Control(pDX, IDC_CHECK_2THREADS, mCheck2Threads);
	DDX_Control(pDX, IDC_CHECK_EWMA, mCheckEWMA);
	DDX_Control(pDX, IDC_SLIDER_EWMA, mSliderEWMA);
	DDX_Control(pDX, IDC_SLIDER_MARKS, mSliderMarks);
	DDX_Control(pDX, IDC_SLIDER_ACCELERATION, mSliderAcceleration);
	DDX_Control(pDX, IDC_SLIDER_FACE_SENSITIVITY, mSliderFace);
	DDX_Control(pDX, IDC_STATIC_EWMA, mStaticEWMA);
	DDX_Control(pDX, IDC_STATIC_MARKS, mStaticMarks);
	DDX_Control(pDX, IDC_STATIC_ACCELERATION, mStaticAcceleration);
	DDX_Control(pDX, IDC_STATIC_FACE_SENSITIVITY, mStaticFace);
	DDX_Control(pDX, IDC_SLIDER_2THREADS, mSlider2Threads);
	DDX_Control(pDX, IDC_STATIC_THREADSNUM, mStaticThreadsNum);

}


BEGIN_MESSAGE_MAP(CAdvancedTab, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_FLIPCAMERA, &CAdvancedTab::OnBnClickedCheckFlipcamera)
	ON_BN_CLICKED(IDC_CHECK_SHOWVIDEO, &CAdvancedTab::OnBnClickedCheckShowvideo)
	ON_BN_CLICKED(IDC_CHECK_2THREADS, &CAdvancedTab::OnBnClickedCheck2threads)
	ON_BN_CLICKED(IDC_CHECK_EWMA, &CAdvancedTab::OnBnClickedCheckEwma)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EWMA, &CAdvancedTab::OnNMCustomdrawSliderEwma)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MARKS, &CAdvancedTab::OnNMCustomdrawSliderMarks)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACCELERATION, &CAdvancedTab::OnNMCustomdrawSliderAcceleration)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FACE_SENSITIVITY, &CAdvancedTab::OnNMCustomdrawSliderFaceSensitivity)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_2THREADS, &CAdvancedTab::OnNMCustomdrawSlider2threads)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CAdvancedTab::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CAdvancedTab::OnBnClickedButtonAbout)
END_MESSAGE_MAP()


// CAdvancedTab message handlers


void CAdvancedTab::OnBnClickedCheckFlipcamera()
{
		UpdateData(TRUE);
		if (mCheckFlipCamera.GetCheck() == NULL) flipCameraFlag = false;
		else flipCameraFlag = true;

		::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_FLIPCAMERA, 0);
}


void CAdvancedTab::OnBnClickedCheckShowvideo()
{
	UpdateData(TRUE);
	if (mCheckShowVideo.GetCheck() == NULL) showVideoFlag = false;
	else showVideoFlag = true;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_SHOWVIDEO, 0);
}


void CAdvancedTab::OnBnClickedCheckEwma()
{
	UpdateData(TRUE);
	if (mCheckEWMA.GetCheck() == NULL)
	{
		EWMAFlag = false;
		mSliderEWMA.EnableWindow(0);
	}
	else
	{
		EWMAFlag = true;
		mSliderEWMA.EnableWindow(1);
	}

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_EWMA, 0);
}



void CAdvancedTab::OnBnClickedCheck2threads()
{
	UpdateData(TRUE);
	if (mCheck2Threads.GetCheck() == NULL)
	{
		mSlider2Threads.EnableWindow(0);
		use2ThreadsFlag = false;
		cv::setNumThreads(defaultThreadsNum);
	}
	else
	{
		mSlider2Threads.EnableWindow(1);
		use2ThreadsFlag = true;
	}

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_2THREADS, 0);
}

void CAdvancedTab::OnNMCustomdrawSlider2threads(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!use2ThreadsFlag) return;
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	curThreadsNum = (int) mSlider2Threads.GetPos();
	if (curThreadsNum < 0) curThreadsNum = 0;
	if (curThreadsNum > defaultThreadsNum) curThreadsNum = defaultThreadsNum;

	str2.Format(L"%.*i", 0, curThreadsNum);
	str1 = "Threads num: ";
	mStaticThreadsNum.SetWindowTextW(str1 + str2);

	cv::setNumThreads(curThreadsNum);


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_STATIC_THREADSNUM, 0);

	*pResult = 0;

}


void CAdvancedTab::OnNMCustomdrawSliderEwma(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderEWMA = (float)mSliderEWMA.GetPos() / 10;
	if (iSliderEWMA < 0.01) iSliderEWMA = 0.01;

	str2.Format(L"%.*f", 1, iSliderEWMA);
	str1 = "Filtration threshold (EWMA filter): ";
	mStaticEWMA.SetWindowTextW(str1 + str2);


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_EWMA, 0);

	*pResult = 0;
}


void CAdvancedTab::OnNMCustomdrawSliderMarks(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderMarks = (float)mSliderMarks.GetPos() / 100;
	if (iSliderMarks < 0.3) iSliderMarks = 0.3;

	str2.Format(L"%.*f", 0, iSliderMarks*100);
	str1 = "Percentage of marks moving in one direction: ";
	mStaticMarks.SetWindowTextW(str1 + str2 + "%");


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_MARKS, 0);
	*pResult = 0;
}


void CAdvancedTab::OnNMCustomdrawSliderAcceleration(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderAcceleration = (float)mSliderAcceleration.GetPos() / 10;
	if (iSliderAcceleration < 1.0) iSliderAcceleration = 1.0;
	str2.Format(L"%.*f", 1, iSliderAcceleration);
	str1 = "Acceleration: ";
	mStaticAcceleration.SetWindowTextW(str1 + str2);


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_ACCELERATION, 0);

	*pResult = 0;
}


void CAdvancedTab::OnNMCustomdrawSliderFaceSensitivity(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderFace = (int) mSliderFace.GetPos();
	if (iSliderFace < 2) iSliderFace = 2;
	str2.Format(L"%.*i", 0, iSliderFace);
	str1 = "Face recognition threshold (AI): ";
	mStaticFace.SetWindowTextW(str1 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_FACE_SENSITIVITY, 0);
	*pResult = 0;
}



BOOL CAdvancedTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	if (!use2ThreadsFlag) mSlider2Threads.EnableWindow(0);
	mSliderEWMA.SetRange(1, 10);
	mSliderMarks.SetRange(30, 98);
	mSliderAcceleration.SetRange(10, 60);
	mSliderFace.SetRange(2, 100);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
	
}






void CAdvancedTab::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CAdvancedTab::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnCancel();
}


void CAdvancedTab::OnBnClickedButtonReset()
{
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_BUTTON_RESET, 0);
}


void CAdvancedTab::OnBnClickedButtonAbout()
{

	if (!mAboutDialog.GetSafeHwnd()) // Have we created the dialog?
	{

		mAboutDialog.Create(IDD_ABOUTBOX);

		// hide from taskbar
		mAboutDialog.ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

		mAboutDialog.ShowWindow(SW_SHOW);

	}


}

