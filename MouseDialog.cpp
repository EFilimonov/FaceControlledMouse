// MouseDialog.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "MouseDialog.h"
#include "afxdialogex.h"
#include <thread>


// MouseDialog dialog

IMPLEMENT_DYNAMIC(MouseDialog, CDialogEx)

MouseDialog::MouseDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUSE_DIALOG, pParent)
	, mStaticInfoText(_T(""))
{

}

MouseDialog::~MouseDialog()
{
}

void MouseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_INFOTEXT, mStaticInfoText);
}


BEGIN_MESSAGE_MAP(MouseDialog, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// MouseDialog message handlers



void MouseDialog::OnPaint()
{
	CPaintDC dc(this); 
	dc.SelectObject(pointerCBrush);
	dc.SelectObject(pointerCPen);
	
	dc.Pie(0, 0, pieSize, pieSize, pieSize, pieSize / 2, x_end_pie, y_end_pie);
	
	
}


BOOL MouseDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	brushGray.CreateSolidBrush(grayCol);
	brushWhite.CreateSolidBrush(whiteCol);
	brushGreen.CreateSolidBrush(greenCol);
	brushOrange.CreateSolidBrush(orangeCol);
	brushDeepOrange.CreateSolidBrush(deepOrangeCol);
	brushRed.CreateSolidBrush(redCol);

	penGray.CreatePen(PS_SOLID | PS_COSMETIC, 1, grayCol);
	penWhite.CreatePen(PS_SOLID | PS_COSMETIC, 1, whiteCol);
	penGreen.CreatePen(PS_SOLID | PS_COSMETIC, 1, greenCol);
	penOrange.CreatePen(PS_SOLID | PS_COSMETIC, 1, orangeCol);
	penDeepOrange.CreatePen(PS_SOLID | PS_COSMETIC, 1, deepOrangeCol);
	penRed.CreatePen(PS_SOLID | PS_COSMETIC, 1, redCol);

	pointerCBrush = &brushGray;
	pointerCPen = &penGray;

	CRect infoTextPosition;
	HWND infoTextHWND;

	GetDlgItem(IDC_STATIC_INFOTEXT)->GetWindowRect(infoTextPosition);

	int infoTextX, infoTextY;
	infoTextX = pieSize / 2 - infoTextPosition.Width() / 2;
	infoTextY = pieSize / 2 - infoTextPosition.Height() / 2;
	GetDlgItem(IDC_STATIC_INFOTEXT)->MoveWindow(infoTextX, infoTextY, infoTextPosition.Width(), infoTextPosition.Height(), true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void MouseDialog::rotatePie(double elapsedSeconds, bool isDwell, bool isQuick, float _secSmile, float _sec1x, float _sec2x, float _secAll)//
{

//	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);



	if (isDwell)
	{
		if (elapsedSeconds <= _secAll)
		{
			if (mouseClick == ONE_CL || mouseClick == RIGHT_CL || mouseClick == DRAG)changePie(ONE_CLICK);
			if (mouseClick == DOUBLE_CL)changePie(DOUBLE);
		}
		else changePie(NEUTRAL);
	}
	else 
	{ 
		if(isQuick) // w/o cancel
		{ 
			if (elapsedSeconds > _secAll) return;
			changePie(SMILING);
		}
		else // with cancel
		{ 
			
		if (elapsedSeconds > _secAll) return;

			if (mouseClick == ONE_CL || mouseClick == RIGHT_CL || mouseClick == DRAG)
			{
				if (elapsedSeconds > 0 && elapsedSeconds <= _secSmile)changePie(SMILING);
				else if (elapsedSeconds > _secSmile && elapsedSeconds <= _sec1x) changePie(ONE_CLICK);
				else if (elapsedSeconds > _sec1x) changePie(CANCEL);
				else changePie(NEUTRAL);
			}
			else if (mouseClick == DOUBLE_CL)
			{
				if (elapsedSeconds > 0 && elapsedSeconds <= _secSmile)changePie(SMILING);
				else if (elapsedSeconds > _secSmile && elapsedSeconds <= _sec1x) changePie(ONE_CLICK);
				else if (elapsedSeconds > _sec1x && elapsedSeconds <= _sec2x) changePie(DOUBLE);
				else if (elapsedSeconds > _sec2x) changePie(CANCEL);
				else changePie(NEUTRAL);
			}
		}
	}

		pieDeg = elapsedSeconds / _secAll;

		pieRad = pieDeg * 3.14159 * 2;

		x_end_pie = pieSize + pieSize * cos(pieRad);

		y_end_pie = pieSize + pieSize * sin(pieRad);

}


void MouseDialog::changePie(ClicksFaces _click)
{
	
	if (_click != click)
	{
		switch (_click)
		{

		case LOSTFACE:
			mStaticInfoText = (L"\U0001F576");// sunglass https://codepoints.net/emoticons
			pointerCBrush = &brushGray;
			pointerCPen = &penGray;
			x_end_pie = pieSize;
			y_end_pie = pieSize / 2;
			RedrawWindow();
			break;
		case NEUTRAL:
			mStaticInfoText = (L"\U0001F610");// neutral https://codepoints.net/emoticons
			pointerCBrush = &brushWhite;
			pointerCPen = &penWhite;
			x_end_pie = pieSize;
			y_end_pie = pieSize / 2;
			RedrawWindow();
			break;
		case SMILING:
			mStaticInfoText = (L"\U0001F642"); // smiling
			pointerCBrush = &brushGreen;
			pointerCPen = &penGreen;
			break;
		case DWELL:
			mStaticInfoText = (L"\U0001F610");// neutral https://codepoints.net/emoticons
			pointerCBrush = &brushGreen;
			pointerCPen = &penGreen;
			break;
		case ONE_CLICK:
			mStaticInfoText = (L"\u2460");// one
			pointerCBrush = &brushOrange;
			pointerCPen = &penOrange;
			break;
		case DOUBLE:
			mStaticInfoText = (L"\u2461");// two
			pointerCBrush = &brushDeepOrange;
			pointerCPen = &penDeepOrange;
			break;
		case CANCEL:
			mStaticInfoText = (L"\u274C");//cancel
			pointerCBrush = &brushRed;
			pointerCPen = &penRed;
			break;
		case PAUSE:
			mStaticInfoText = (L"\u23F8");
			pointerCBrush = &brushGray;
			pointerCPen = &penGray;
			break;
		default:
			mStaticInfoText = (L"");
			pointerCBrush = &brushGray;
			pointerCPen = &penGray;
			break;
		}
		UpdateData(FALSE);
		click = _click;
	}
	else return;
}


HBRUSH MouseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}


void MouseDialog::moveMouseDlg()
{

	GetCursorPos(&cursorPos);


	dlgRect.left = cursorPos.x + 10;
	dlgRect.right = cursorPos.x + 10 + pieSize;
	dlgRect.top = cursorPos.y - 10;
	dlgRect.bottom = cursorPos.y - 10 + pieSize;
	MoveWindow(dlgRect);
	


}




void MouseDialog::MouseInput()
{
	const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
	const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);


	GetCursorPos(&cursorPos);

	double cx = cursorPos.x * XSCALEFACTOR;
	double cy = cursorPos.y * YSCALEFACTOR;


	if (mouseClick == ONE_CL)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, cx, cy, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
		mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, cx, cy, 0, 0);
	}

	if (mouseClick == DOUBLE_CL)
	{
		if (isTimer2Clicks)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, cx, cy, 0, 0);
		}

		else
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTDOWN, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, cx, cy, 0, 0);
		}

	}

	if (mouseClick == RIGHT_CL)
	{

		// return control to the left click
		CWnd* m_pMainWnd;
		HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
		::SendMessage(hWnd, UWM_CUSTOMRIGHTCLICK, 0, 0);

		if (isNonMainClientArea || isNonOptionsClientArea) return;

		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN, cx, cy, 0, 0);
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP, cx, cy, 0, 0);

	}
	if (mouseClick == DRAG)
	{
		//if (isNonClientArea) return;
		if (notDragFlag)
		{
			notDragFlag = false;
			if (isNonMainClientArea || isNonOptionsClientArea) return;

			mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, cx, cy, 0, 0);

		}
		else
		{
			notDragFlag = true;

			// return control to the left click
			CWnd* m_pMainWnd;
			HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
			::SendMessage(hWnd, UWM_CUSTOMRIGHTCLICK, 0, 0);
			if (isNonMainClientArea || isNonOptionsClientArea) return;

			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, cx, cy, 0, 0);

		}
	}
}