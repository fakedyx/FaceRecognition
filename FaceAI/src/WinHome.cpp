// WinHome.cpp: 实现文件
//

#include "pch.h"
#include "FaceAi.h"
#include "afxdialogex.h"
#include "WinHome.h"
#include <graphics.h>
#include "tools.h"


// WinHome 对话框

IMPLEMENT_DYNAMIC(WinHome, CDialogEx)

WinHome::WinHome(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACEAI_HOME, pParent)
{

}

WinHome::~WinHome()
{
}

BOOL WinHome::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//生成带水印的图片
	char fileName[256];
	sprintf_s(fileName, sizeof(fileName), "pic/users/%s-%s-%d.jpg", m_job, m_username, m_yanzhi);

	IMAGE img1, img2;
	loadimage(&img1, CString(fileName), 200, 150, true);
	loadimage(&img2, L"res/yz2.png");

	putimagePNG(0, 0, &img2, &img1);
	saveimage(L"pic/com/hc.jpg", &img1);
	
	imgR.Load(L"pic/com/hc.jpg");
	m_img_head.SetBitmap(imgR);

	m_font.CreatePointFont(150, L"YAHEI CONSOLAS HYBRID");

	m_text_name.SetFont(&m_font);
	m_text_name.SetWindowText((CString)m_username);
	
	CString yanzhi;
	yanzhi.Format(L"颜值：%d", m_yanzhi);
	m_text_yanzhi.SetFont(&m_font);
	m_text_yanzhi.SetWindowText(yanzhi);

	m_text_job.SetFont(&m_font);
	m_text_job.SetWindowText((CString)m_job);

	m_text_b_bi.SetFont(&m_font);
	m_text_b_bi.SetWindowText(L"B币：999");

	m_text_qq.SetFont(&m_font);
	m_text_qq.SetWindowText(L"QQ：2499146889");

	m_bgLeft.Load(L"res/buttonBg.bmp");
	m_btn_play_stop.Init(IDB_PNG6, 4, BTN_TYPE_NORMAL);

	videoPlayerInit(&m_player);  //初始化播放器
	srand(time(NULL));
	return 0;
}

void WinHome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMG_HEAD, m_img_head);
	DDX_Control(pDX, IDC_TEXT_NAME, m_text_name);
	DDX_Control(pDX, IDC_TEXT_YANZHI, m_text_yanzhi);
	DDX_Control(pDX, IDC_TEXT_JOB, m_text_job);
	DDX_Control(pDX, IDC_TEXT_B_BI, m_text_b_bi);
	DDX_Control(pDX, IDC_TEXT_QQ, m_text_qq);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_play_stop);
}


BEGIN_MESSAGE_MAP(WinHome, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &WinHome::OnBnClickedButton1)
END_MESSAGE_MAP()


// WinHome 消息处理程序


void WinHome::OnPaint()
{
	drawPicOnPait(&m_bgLeft, this, 450, 0);
}


void WinHome::OnBnClickedButton1()
{
	char fileName[64];
	int num;
	if (status == 0) {
		/*播放*/
		m_player.hwnd = GetDlgItem(IDC_IMG_MOVIE)->GetSafeHwnd();
		num = rand() % 2;
		sprintf_s(fileName, sizeof(fileName), "res/movie/movie%d.mp4", num);
		videoPlayerPlay(&m_player, fileName);
		status = 1;
		m_btn_play_stop.Init(IDB_PNG7, 4, BTN_TYPE_NORMAL);
	}
	else if (status == 1) {
		/*暂停*/
		videoPlayerPause(&m_player);
		m_btn_play_stop.Init(IDB_PNG6, 4, BTN_TYPE_NORMAL);
		status = 2;
	}
	else {
		/*继续播放*/
		videoPlayerPause(&m_player);
		m_btn_play_stop.Init(IDB_PNG7, 4, BTN_TYPE_NORMAL);
		status = 1;
	}
}
