// MouseDialog.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "MouseDialog.h"
#include "afxdialogex.h"
#include <thread>

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}




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


	////////////////////////////////////

	secToOneClickDuration = secSmile + secOneClick;
	secToDoubleClickDuration = secSmile + secOneClick + secDoubleClick;
	secToOneClickCancelDuration = secSmile + secOneClick + secCancel;
	secToDoubleClickCancelDuration = secSmile + secOneClick + secDoubleClick + secCancel;

	////////////////////////////////////



// Sounds

	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);	

	IXAudio2* pXAudio2 = nullptr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) { playSoundFlag = false; return TRUE; }

	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) { playSoundFlag = false; return TRUE; }

	WAVEFORMATEXTENSIBLE wfx = { 0 };


	const TCHAR* strFileName = TEXT("click.wav");

	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile) { playSoundFlag = false; return TRUE; }
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) { playSoundFlag = false; return TRUE; }
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE) { playSoundFlag = false; return TRUE; }
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer


	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) { playSoundFlag = false; return TRUE; };
	//if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer))) { playSoundFlag = false; return TRUE; };
	//if (FAILED(hr = pSourceVoice->Start(0))) { playSoundFlag = false; return TRUE; };

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
			RedrawWindow();
			break;
		case DWELL:
			mStaticInfoText = (L"\U0001F610");// neutral https://codepoints.net/emoticons
			pointerCBrush = &brushGreen;
			pointerCPen = &penGreen;
			RedrawWindow();
			break;
		case ONE_CLICK:
			mStaticInfoText = (L"\u2460");// one
			pointerCBrush = &brushOrange;
			pointerCPen = &penOrange;
			RedrawWindow();
			break;
		case DOUBLE:
			mStaticInfoText = (L"\u2461");// two
			pointerCBrush = &brushDeepOrange;
			pointerCPen = &penDeepOrange;
			RedrawWindow();
			break;
		case CANCEL:
			mStaticInfoText = (L"\u274C");//cancel
			pointerCBrush = &brushRed;
			pointerCPen = &penRed;
			RedrawWindow();
			break;
		case PAUSE:
			mStaticInfoText = (L"\u23F8");
			pointerCBrush = &brushGray;
			pointerCPen = &penGray;
			x_end_pie = pieSize;
			y_end_pie = pieSize / 2;
			RedrawWindow();
			break;
		default:
			mStaticInfoText = (L"");
			pointerCBrush = &brushGray;
			pointerCPen = &penGray;
			RedrawWindow();
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


void MouseDialog::moveMouseDlg(bool onTop)
{

	GetCursorPos(&cursorPos);


	dlgRect.left = cursorPos.x + 10;
	dlgRect.right = cursorPos.x + 10 + pieSize;
	dlgRect.top = cursorPos.y - 10;
	dlgRect.bottom = cursorPos.y - 10 + pieSize;

	if (onTop)
	{
	SetWindowPos(&CWnd::wndTopMost,
		cursorPos.x + 10,
		cursorPos.y - 10,
		cursorPos.x + 10 + pieSize,
		cursorPos.y - 10 + pieSize,
		SWP_NOSIZE);
	}
	else 
	MoveWindow(dlgRect);

}




void MouseDialog::MouseInput()
{
	const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
	const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);

	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	GetCursorPos(&cursorPos);

	double cx = cursorPos.x * XSCALEFACTOR;
	double cy = cursorPos.y * YSCALEFACTOR;

	playClickSound();

	::SendMessage(hWnd, UWM_CUSTOMCLICK, 0, 0);

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
			::SendMessage(hWnd, UWM_CUSTOMRIGHTCLICK, 0, 0);
			if (isNonMainClientArea || isNonOptionsClientArea) return;

			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, cx, cy, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, cx, cy, 0, 0);



		}
	}
	//::SendMessage(hWnd, UWM_CUSTOMCLICK, 0, 0);
}

void MouseDialog::playClickSound()
{
	if (playSoundFlag && needSound)
	{
		hr = pSourceVoice->SubmitSourceBuffer(&buffer);
		hr = pSourceVoice->Start(0);
	}
}


void MouseDialog::dwellDetecting(float dist)
{

//	inptext = "secDwellStartTime = " + std::to_string(secDwellStartTime) + "elapsedSeconds = " + std::to_string(elapsedSeconds) + " dwellMouseLocked = " + std::to_string(dwellMouseLocked);

	if (dist < dwellDisp)
	{

		if (dwellMouseLocked)
		{
			dwellTimer.start();
			if (!IsWindowVisible()) ShowWindow(SW_SHOW);
		}

		dwellMouseLocked = false;

		elapsedSeconds = dwellTimer.elapsedSeconds();


		if (elapsedSeconds > secDwellStartTime)
		{
			if (elapsedSeconds < secDwellStartTime + dwellDuration)
			{
				rotatePie(elapsedSeconds - secDwellStartTime, true, false, secSmile, secToOneClickDuration, secToDoubleClickDuration, dwellDuration);
				RedrawWindow();
			}
			else
			{
				MouseInput();
				dwellTimer.stop();
				changePie(NEUTRAL);
				RedrawWindow();
				dwellMouseLocked = true;
			}
		}

	}
	else
	{
		dwellTimer.stop();
		changePie(NEUTRAL);
		RedrawWindow();
		dwellMouseLocked = true;
	}

}


void MouseDialog::quickMouseDlg(bool detect)
{
	if (detect)

	{
		// break if recent smile
//		if (!quickSmileUnlocked) return;

		if (smileMouseLocked)
		{
			mouseTimer.start();
		}

		smileMouseLocked = false;


		elapsedSeconds = mouseTimer.elapsedSeconds();

		if (elapsedSeconds < secQuickClick)
		{
			rotatePie(elapsedSeconds, false, true, secSmile, secToOneClickDuration, secToDoubleClickDuration, secQuickClick);
			RedrawWindow();
		}
		else // if keep smiling
		{
			MouseInput();
			mouseTimer.stop();
			changePie(NEUTRAL);
			RedrawWindow();
			smileMouseLocked = false;
		//	quickSmileUnlocked = true;
		}

	}
	else
	{
		mouseTimer.stop();
		changePie(NEUTRAL);
		RedrawWindow();
		smileMouseLocked = true;
		//quickSmileUnlocked = true;
	}

}


void MouseDialog::timerMouseDlg(bool detect)
{

	if (detect)
	{

		if (smileMouseLocked)
		{
			initiateStart();
		}

		smileMouseLocked = false;


		elapsedSeconds = mouseTimer.elapsedSeconds();

		if (elapsedSeconds < secDuration)
		{
			rotatePie(elapsedSeconds, false, false, secSmile, secToOneClickDuration, secToDoubleClickDuration, secDuration);

			// if keep smiling
			if (elapsedSeconds > secSmile)
			{
				// if one-click settings
				if (mouseClick == ONE_CL)
				{
					mouseClick = ONE_CL;
					if (elapsedSeconds > secToOneClickDuration)
						mouseClick = DO_NOTHING;

				}
				// if double-click settings
				else if (mouseClick == DOUBLE_CL)
				{
					mouseClick = ONE_CL;
					if (elapsedSeconds > secToDoubleClickDuration)
						mouseClick = DOUBLE_CL;
					if (elapsedSeconds > secToDoubleClickCancelDuration)
						mouseClick = DO_NOTHING;
				}
				// if left-click settings
				else if (mouseClick == RIGHT_CL)
				{
					mouseClick = RIGHT_CL;
					if (elapsedSeconds > secToOneClickDuration)
						mouseClick = DO_NOTHING;

				}
				else if (mouseClick == DRAG)
				{
					mouseClick = DRAG;
					if (elapsedSeconds > secToOneClickDuration)
						mouseClick = DO_NOTHING;
				}

			}

		}

		else mouseTimer.stop();
		RedrawWindow();

	}
	else
	{
		if (mouseClick != DO_NOTHING)
		{
			MouseInput();
			mouseClick = DO_NOTHING;
		}
		mouseTimer.stop();
		smileMouseLocked = true;
		changePie(NEUTRAL);
		RedrawWindow();
	}

}

void MouseDialog::initiateStart()
{
	secToOneClickDuration = secSmile + secOneClick;
	secToDoubleClickDuration = secSmile + secOneClick + secDoubleClick;
	secToOneClickCancelDuration = secSmile + secOneClick + secCancel;
	secToDoubleClickCancelDuration = secSmile + secOneClick + secDoubleClick + secCancel;
	if (mouseClick == ONE_CL) secDuration = secToOneClickCancelDuration;
	if (mouseClick == DOUBLE_CL) secDuration = secToDoubleClickCancelDuration;
	mouseTimer.start();
}

