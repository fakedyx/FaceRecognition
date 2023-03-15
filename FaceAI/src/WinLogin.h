#pragma once
#include "afxdialogex.h"
#include "ButtonPNG.h"

// WinLogin 对话框

class WinLogin : public CDialogEx
{
	DECLARE_DYNAMIC(WinLogin)

public:
	WinLogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinLogin();
	BOOL OnInitDialog();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACEAI_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ButtonPNG m_btnMouse;
	ATL::CImage m_imgBG;
	afx_msg void OnPaint();

	HBITMAP m_snows[16];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_snow;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonLog();
	CStatic m_photo;

	char m_job[64];
	int m_score;
	char m_username[64];
	bool m_logined;
};
