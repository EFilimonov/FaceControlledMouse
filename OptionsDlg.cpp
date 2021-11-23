// OptionsDlg.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"
#include "FaceControllerMFCDlg.h"



// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialogEx)

OptionsDlg::OptionsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{

}

OptionsDlg::~OptionsDlg()
{
}

void OptionsDlg::DoDataExchange(CDataExchange* pDX)
{	
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_ctrTab);
}


BEGIN_MESSAGE_MAP(OptionsDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &OptionsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY, &OptionsDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDCANCEL, &OptionsDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &OptionsDlg::OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &OptionsDlg::OnSelchangingTab)
	ON_WM_NCMOUSELEAVE()
	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()


// OptionsDlg message handlers

void OptionsDlg::OnClose()
{
	pCMainTab.fSliderSpeedHorisontal = back_fSliderSpeedHorisontal;
	pCMainTab.fSliderSpeedVertical = back_fSliderSpeedVertical;
	pCMainTab.iSliderSmileAngle = back_iSliderSmileAngle;
	pCMainTab.iSliderSmileSensitivity = back_iSliderSmileSensitivity;
	pCMainTab.fSliderDwellDispl = back_fSliderDwellDispl;
	pCMainTab.fSliderDwellTime = back_fSliderDwellTime;
	pCMainTab.fSliderQuickTime = back_fSliderQuickTime;
	pCMainTab.fSlider1x = back_fSlider1x;
	pCMainTab.fSlider2x = back_fSlider2x;
	pCMainTab.fSliderCancel = back_fSliderCancel;
	pCMainTab.fSliderTimeToDwell = back_fSliderTimeToDwell;

	pCAdvancedTab.flipCameraFlag = back_flipCameraFlag;
	pCAdvancedTab.needEqualize = back_needEqualize;
	pCAdvancedTab.fSliderEWMA = back_fSliderEWMA;
	pCAdvancedTab.fSliderMarks = back_fSliderMarks;
	pCAdvancedTab.fSliderAcceleration = back_fSliderAcceleration;
	pCAdvancedTab.iSliderFace = back_iSliderFace;
	pCAdvancedTab.mEditKeyboard = back_mEditKeyboard;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDCANCEL, 0);

	CDialogEx::OnClose();
}


void OptionsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void OptionsDlg::OnBnClickedApply()
{

	UpdateData(TRUE);
	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDC_APPLY, 0);
}


void OptionsDlg::OnBnClickedCancel()
{

	pCMainTab.fSliderSpeedHorisontal = back_fSliderSpeedHorisontal;
	pCMainTab.fSliderSpeedVertical = back_fSliderSpeedVertical;
	pCMainTab.iSliderSmileAngle = back_iSliderSmileAngle;
	pCMainTab.iSliderSmileSensitivity = back_iSliderSmileSensitivity;
	pCMainTab.fSliderDwellDispl = back_fSliderDwellDispl;
	pCMainTab.fSliderDwellTime = back_fSliderDwellTime;
	pCMainTab.fSliderQuickTime = back_fSliderQuickTime;
	pCMainTab.fSlider1x = back_fSlider1x;
	pCMainTab.fSlider2x = back_fSlider2x;
	pCMainTab.fSliderCancel = back_fSliderCancel;
	pCMainTab.fSliderTimeToDwell = back_fSliderTimeToDwell;

	pCAdvancedTab.flipCameraFlag = back_flipCameraFlag;
	pCAdvancedTab.needEqualize = back_needEqualize;
	pCAdvancedTab.fSliderEWMA = back_fSliderEWMA;
	pCAdvancedTab.fSliderMarks = back_fSliderMarks;
	pCAdvancedTab.fSliderAcceleration = back_fSliderAcceleration;
	pCAdvancedTab.iSliderFace = back_iSliderFace;
	pCAdvancedTab.mEditKeyboard = back_mEditKeyboard;

	::SendMessage(hWnd, UWM_UPDATE_OPTIONS, (WPARAM)IDCANCEL, 0);

	CDialogEx::OnCancel();
}



BOOL OptionsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TC_ITEM TabItem;
	TabItem.mask = TCIF_TEXT;
	TabItem.pszText = _T("Main");
	m_ctrTab.InsertItem(0, &TabItem);
	TabItem.pszText = _T("Advanced");
	m_ctrTab.InsertItem(1, &TabItem);

	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = (LPARAM)&pCMainTab;
	m_ctrTab.SetItem(0, &TabItem);
	VERIFY(pCMainTab.Create(IDD_MAIN_TAB, &m_ctrTab));
	pCMainTab.SetWindowPos(NULL, 0, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	pCMainTab.ShowWindow(SW_SHOW);


	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = (LPARAM)&pCAdvancedTab;
	m_ctrTab.SetItem(1, &TabItem);
	VERIFY(pCAdvancedTab.Create(IDD_ADVANCED_TAB, &m_ctrTab));
	pCAdvancedTab.SetWindowPos(NULL, 0, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	pCAdvancedTab.ShowWindow(SW_HIDE);

	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;





	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void OptionsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	
}


void OptionsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	pCMainTab.DestroyWindow();
	pCAdvancedTab.DestroyWindow();

}


void OptionsDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_ctrTab.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	m_ctrTab.GetItem(nTab, &tci);
	ASSERT(tci.lParam);

	CWnd* pWnd = (CWnd*)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);


	*pResult = 0;
}


void OptionsDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nTab = m_ctrTab.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	m_ctrTab.GetItem(nTab, &tci);
	ASSERT(tci.lParam);

	CWnd* pWnd = (CWnd*)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);

	*pResult = 0;
}



void OptionsDlg::OnNcMouseLeave()
{
	mMouseDlg->isNonOptionsClientArea = false;

	CDialogEx::OnNcMouseLeave();
}


void OptionsDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	mMouseDlg->isNonOptionsClientArea = true;

	CDialogEx::OnNcMouseMove(nHitTest, point);
}
