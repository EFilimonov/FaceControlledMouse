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
	, mEditKeyboard(_T(""))
{

}

CAdvancedTab::~CAdvancedTab()
{
}

void CAdvancedTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_FLIPCAMERA, mCheckFlipCamera);
	DDX_Control(pDX, IDC_SLIDER_EWMA, mSliderEWMA);
	//	DDX_Control(pDX, IDC_SLIDER_MARKS, mSliderMarks);
	DDX_Control(pDX, IDC_SLIDER_ACCELERATION, mSliderAcceleration);
	DDX_Control(pDX, IDC_SLIDER_FACE_SENSITIVITY, mSliderFace);
	DDX_Control(pDX, IDC_STATIC_EWMA, mStaticEWMA);
	//	DDX_Control(pDX, IDC_STATIC_MARKS, mStaticMarks);
	DDX_Control(pDX, IDC_STATIC_ACCELERATION, mStaticAcceleration);
	DDX_Control(pDX, IDC_STATIC_FACE_SENSITIVITY, mStaticFace);
	DDX_Text(pDX, IDC_EDIT_KEYBOARD, mEditKeyboard);
	DDX_Control(pDX, IDC_CHECK_EQUALIZE, mCheckEqualize);
	DDX_Control(pDX, IDC_SLIDER_FACE_MINNUM, mSliderFaceMinNum);
	//	DDX_Control(pDX, IDC_SLIDER_FACE_MINDIST, mSliderFaceMaxNum);
	DDX_Control(pDX, IDC_STATIC_MINNUM, mStaticMinNum);
	//DDX_Control(pDX, IDC_STATIC_MINDIST, mStaticMaxNum);
	DDX_Control(pDX, IDC_SLIDER_CUSTOM, mSliderCustom);
	DDX_Control(pDX, IDC_STATIC_CUSTOM, mStaticCustom);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, mCheckAutostart);
	DDX_Control(pDX, IDC_SLIDER_ONINPUT, mSliderOnInput);
	DDX_Control(pDX, IDC_STATIC_ONINPUT, mStaticOnInput);
	DDX_Control(pDX, IDC_CHECK_SOUND, mCheckSound);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, mComboLanguage);
	DDX_Control(pDX, IDC_STATIC_LANGUAGES, mStaticLanguages);
	DDX_Control(pDX, IDC_CHECK_THREADING, mCheckThreading);
	DDX_Control(pDX, IDC_STATIC_FACE_FRAMES, mStaticFaceFrames);
	DDX_Control(pDX, IDC_SLIDER_FACE_FRAMES, mSliderFaceFrames);
}


BEGIN_MESSAGE_MAP(CAdvancedTab, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_FLIPCAMERA, &CAdvancedTab::OnBnClickedCheckFlipcamera)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EWMA, &CAdvancedTab::OnNMCustomdrawSliderEwma)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MARKS, &CAdvancedTab::OnNMCustomdrawSliderMarks)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACCELERATION, &CAdvancedTab::OnNMCustomdrawSliderAcceleration)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FACE_SENSITIVITY, &CAdvancedTab::OnNMCustomdrawSliderFaceSensitivity)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CAdvancedTab::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CAdvancedTab::OnBnClickedButtonAbout)
	ON_EN_CHANGE(IDC_EDIT_KEYBOARD, &CAdvancedTab::OnEnChangeEditKeyboard)
	ON_BN_CLICKED(IDC_CHECK_EQUALIZE, &CAdvancedTab::OnBnClickedCheckEqualize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FACE_MINNUM, &CAdvancedTab::OnNMCustomdrawSliderFaceMinnum)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FACE_MINDIST, &CAdvancedTab::OnNMCustomdrawSliderFaceMaxnum)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CUSTOM, &CAdvancedTab::OnNMCustomdrawSliderCustom)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CAdvancedTab::OnBnClickedCheckAutostart)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ONINPUT, &CAdvancedTab::OnNMCustomdrawSliderOninput)
	ON_BN_CLICKED(IDC_CHECK_SOUND, &CAdvancedTab::OnBnClickedCheckSound)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CAdvancedTab::OnCbnSelchangeComboLanguage)
	ON_BN_CLICKED(IDC_CHECK_THREADING, &CAdvancedTab::OnBnClickedCheckThreading)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FACE_FRAMES, &CAdvancedTab::OnNMCustomdrawSliderFaceFrames)
END_MESSAGE_MAP()



BOOL CAdvancedTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	mSliderEWMA.SetRange(1, 60);
//	mSliderMarks.SetRange(30, 98);
	mSliderAcceleration.SetRange(10, 50);
	mSliderFace.SetRange(2, 100);
//	mSliderFaceMaxNum.SetRange(10, 25);
	mSliderFaceMinNum.SetRange(3, 15);
	mSliderCustom.SetRange(1, 10);
	mSliderOnInput.SetRange(0.01, 100);
	mSliderFaceFrames.SetRange(1, 10);

	if (IsMyProgramRegisteredForStartup(L"Face_Controlled_Mouse"))
	{
		mCheckAutostart.SetCheck(TRUE);
	}
	else
	{
		mCheckAutostart.SetCheck(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE

}


void CAdvancedTab::RegisterProgram()
{
	wchar_t pathToExe[MAX_PATH];
	PCWSTR pszAppName = L"Face_Controlled_Mouse";
	PCWSTR args = L"-foobar";
	//PCWSTR args = L"-foobar";
	GetModuleFileNameW(NULL, pathToExe, MAX_PATH);


	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");


	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = (wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}


}

void CAdvancedTab::DeleteProgram()
{
	LONG lResult;
	const wchar_t* run_key_path = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;
	PCWSTR pszAppName = L"Face_Controlled_Mouse";
	lResult = RegOpenKeyW(HKEY_CURRENT_USER, run_key_path, &hKey);

	if (lResult == ERROR_SUCCESS)
	{
		lResult = RegDeleteValueW(hKey, pszAppName);
		if (lResult != ERROR_SUCCESS)AfxMessageBox(L"Can't access registration");

	}
	else AfxMessageBox(L"Can't access registration");



}
void CAdvancedTab::OnBnClickedCheckFlipcamera()
{
	UpdateData(TRUE);
	if (mCheckFlipCamera.GetCheck() == NULL) flipCameraFlag = false;
	else flipCameraFlag = true;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_FLIPCAMERA, 0);
}


void CAdvancedTab::OnNMCustomdrawSliderOninput(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	fSliderOnInput = (float)mSliderOnInput.GetPos() / 10;
	if (fSliderOnInput < 0.01) fSliderOnInput = 0.01;

	str2.Format(L"%.*f", 1, fSliderOnInput);
	mStaticOnInput.SetWindowTextW(statictext22 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_ONINPUT, 0);

	*pResult = 0;
}


void CAdvancedTab::OnNMCustomdrawSliderEwma(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	fSliderEWMA = (float)mSliderEWMA.GetPos() / 100;
	if (fSliderEWMA < 0.01) fSliderEWMA = 0.01;

	str2.Format(L"%.*f", 2, fSliderEWMA);
	//str1 = "Smoothing threshold: ";
	mStaticEWMA.SetWindowTextW(statictext23 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_EWMA, 0);

	*pResult = 0;
}


/*
void CAdvancedTab::OnNMCustomdrawSliderMarks(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	fSliderMarks = (float)mSliderMarks.GetPos() / 100;
	if (fSliderMarks < 0.3) fSliderMarks = 0.3;

	str2.Format(L"%.*f", 0, fSliderMarks * 100);
	//str1 = "% of marks moving in one direction: ";
	mStaticMarks.SetWindowTextW(statictext24 + str2 + "%");


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_MARKS, 0);
	*pResult = 0;
}*/


void CAdvancedTab::OnNMCustomdrawSliderAcceleration(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	fSliderAcceleration = (float)mSliderAcceleration.GetPos() / 10;
	if (fSliderAcceleration < 1.0) fSliderAcceleration = 1.0;
	str2.Format(L"%.*f", 1, fSliderAcceleration);
//	str1 = "Acceleration: ";
	mStaticAcceleration.SetWindowTextW(statictext25 + str2);


	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_ACCELERATION, 0);

	*pResult = 0;
}


void CAdvancedTab::OnNMCustomdrawSliderFaceSensitivity(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderFace = (int)mSliderFace.GetPos();
	if (iSliderFace < 2) iSliderFace = 2;
	str2.Format(L"%.*i", 0, iSliderFace);
//	str1 = "Threshold (AI): ";
	mStaticFace.SetWindowTextW(statictext27 + str2);

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_FACE_SENSITIVITY, 0);
	*pResult = 0;
}



void CAdvancedTab::OnOK()
{


	//CDialogEx::OnOK();
}


void CAdvancedTab::OnCancel()
{


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


void CAdvancedTab::OnEnChangeEditKeyboard()
{
	UpdateData(TRUE);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_EDIT_KEYBOARD, 0);
}


void CAdvancedTab::OnBnClickedCheckEqualize()
{
	UpdateData(TRUE);
	if (mCheckEqualize.GetCheck() == NULL) needEqualize = false;
	else needEqualize = true;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_EQUALIZE, 0);
}

void CAdvancedTab::OnBnClickedCheckSound()
{
	UpdateData(TRUE);
	if (mCheckSound.GetCheck() == NULL) needSound = false;
	else needSound = true;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_SOUND, 0);

}

void CAdvancedTab::OnNMCustomdrawSliderFaceMinnum(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderMinNum = mSliderFaceMinNum.GetPos();
	if (iSliderMinNum < 3) iSliderMinNum = 3;
//	if (iSliderMinNum > iSliderMaxNum - 2) iSliderMinNum > iSliderMaxNum - 2;
	str2.Format(L"%.*i", 0, iSliderMinNum);
//	str1 = "Min marks: ";
	mStaticMinNum.SetWindowTextW(statictext29 + str2);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_FACE_MINNUM, 0);

	*pResult = 0;
}



void CAdvancedTab::OnNMCustomdrawSliderFaceFrames(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderFaceFrames = mSliderFaceFrames.GetPos();
	if (iSliderFaceFrames < 1) iSliderFaceFrames = 1;
	if (iSliderFaceFrames > 10) iSliderFaceFrames = 10;
	str2.Format(L"%.*i", 0, iSliderFaceFrames);
	mStaticFaceFrames.SetWindowTextW(statictext30 + str2);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_FACE_FRAMES, 0);

	*pResult = 0;
}


/*
void CAdvancedTab::OnNMCustomdrawSliderFaceMaxnum(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderMaxNum = mSliderFaceMaxNum.GetPos();
	if (iSliderMaxNum < 10) iSliderMaxNum = 10;
	if (iSliderMaxNum < iSliderMinNum + 2) iSliderMaxNum = iSliderMinNum + 2;
	str2.Format(L"%.*i", 0, iSliderMaxNum);
//	str1 = "Max marks: ";
	mStaticMaxNum.SetWindowTextW(statictext30 + str2);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_FACE_MINDIST, 0);

	*pResult = 0;
}
*/

void CAdvancedTab::OnNMCustomdrawSliderCustom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	iSliderCustom = mSliderCustom.GetPos();
	if (iSliderCustom < 1) iSliderCustom = 1;
	str2.Format(L"%.*i", 0, iSliderCustom);
//	str1 = "Cells/width: ";
	mStaticCustom.SetWindowTextW(statictext28 + str2);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_SLIDER_CUSTOM, 0);
	*pResult = 0;
}


BOOL CAdvancedTab::IsMyProgramRegisteredForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;
	wchar_t szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(szPathToExe);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		lResult = RegGetValueW(hKey, NULL, pszAppName, RRF_RT_REG_SZ, &dwRegType, szPathToExe, &dwSize);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		fSuccess = (wcslen(szPathToExe) > 0) ? TRUE : FALSE;
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}




void CAdvancedTab::OnBnClickedCheckAutostart()
{
	UpdateData(TRUE);
	if (mCheckAutostart.GetCheck() == NULL)
	{
		DeleteProgram();
		needAutostart = false;
	}
	else
	{
		needAutostart = true;
		RegisterProgram();
		IsMyProgramRegisteredForStartup(L"Face_Controlled_Mouse");
	}

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_AUTOSTART, 0);
}





void CAdvancedTab::OnCbnSelchangeComboLanguage()
{
	UpdateData(TRUE);
	langNum = mComboLanguage.GetCurSel();
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_COMBO_LANGUAGE, 0);
}


void CAdvancedTab::OnBnClickedCheckThreading()
{
	//enableMultithreading
	// TODO: Add your control notification handler code here
	/*

	if (mCheckSound.GetCheck() == NULL) needSound = false;
	else needSound = true;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_SOUND, 0);
	
	*/
		UpdateData(TRUE);
		if (mCheckThreading.GetCheck() == NULL) enableMultithreading = false;
		else enableMultithreading = true;

		::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_CHECK_THREADING, 0);
		

}
