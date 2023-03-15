#pragma once
#include "afxdialogex.h"
#include "ButtonPNG.h"
#include "VideoPlayer.h"

// WinHome 对话框

class WinHome : public CDialogEx
{
	DECLARE_DYNAMIC(WinHome)

public:
	WinHome(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~WinHome();
	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACEAI_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_img_head;
	CStatic m_text_name;
	CStatic m_text_yanzhi;
	CStatic m_text_job;
	CStatic m_text_b_bi;
	CStatic m_text_qq;

	//添加三个成员变量
	char m_username[64];
	int m_yanzhi;
	char m_job[64];

	CFont m_font;
	ATL::CImage imgR;
	ButtonPNG m_btn_play_stop;

	ATL::CImage m_bgLeft;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();

	VideoPlayer m_player;  //播放器
	int status = 0;     //当前播放器状态
};
