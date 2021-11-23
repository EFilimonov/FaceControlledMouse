// AboutDialog.cpp : implementation file
//

#include "pch.h"
#include "FaceControllerMFC.h"
#include "AboutDialog.h"
#include "afxdialogex.h"


// AboutDialog dialog

IMPLEMENT_DYNAMIC(AboutDialog, CDialogEx)

AboutDialog::AboutDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutDialog, CDialogEx)
END_MESSAGE_MAP()


// AboutDialog message handlers
