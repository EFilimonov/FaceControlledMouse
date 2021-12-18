
// FaceControllerMFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFaceControllerMFCApp:
// See FaceControllerMFC.cpp for the implementation of this class
//

class CFaceControllerMFCApp : public CWinApp
{
public:
	CFaceControllerMFCApp();


	void MouseActions();
	void TrackingActions();

	//	UINT __cdecl RecExtractThreadCtrlFunc(LPVOID pParam);

	static void thread_proc1(void* param) {
		CFaceControllerMFCApp* pthis = (CFaceControllerMFCApp*)param;
		if (pthis != 0)
		{
			pthis->TrackingActions();
		}
	}

	static void thread_proc2(void* param) {
		CFaceControllerMFCApp* pthis = (CFaceControllerMFCApp*)param;
		if (pthis != 0)
		{
			pthis->MouseActions();
		}
	}



// Overrides
public:
	virtual BOOL InitInstance();



// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFaceControllerMFCApp theApp;
