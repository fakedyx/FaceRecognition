#pragma once
#include "afxdialogex.h"


// WinWelcome 对话框

class WinWelcome : public CDialogEx
{
	DECLARE_DYNAMIC(WinWelcome)

public:
	WinWelcome(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinWelcome();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACEAI_WELCOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
