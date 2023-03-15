// WinWelcome.cpp: 实现文件
//

#include "pch.h"
#include "FaceAi.h"
#include "afxdialogex.h"
#include "WinWelcome.h"


// WinWelcome 对话框

IMPLEMENT_DYNAMIC(WinWelcome, CDialogEx)

WinWelcome::WinWelcome(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACEAI_WELCOME, pParent)
{

}

WinWelcome::~WinWelcome()
{
}

void WinWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WinWelcome, CDialogEx)
END_MESSAGE_MAP()


// WinWelcome 消息处理程序
