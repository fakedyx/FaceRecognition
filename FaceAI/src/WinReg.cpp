// WinReg.cpp: 实现文件
//

#include "pch.h"
#include "FaceAi.h"
#include "afxdialogex.h"
#include "WinReg.h"
#include <opencv2/opencv.hpp>
#include <mmsystem.h>
#include "faceTool.h"

using namespace cv;
#include "tools.h"
#include "WinWelcome.h"
#pragma comment(lib, "winmm.lib")

static VideoCapture cap; //定义摄像头
extern MHandle faceModel;

// WinReg 对话框

IMPLEMENT_DYNAMIC(WinReg, CDialogEx)

WinReg::WinReg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACEAI_REG, pParent)
{

}

WinReg::~WinReg()
{
}

void WinReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_REG, m_btnMouse);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_SNOW, m_snowBG);
	DDX_Control(pDX, IDC_PHOTO_REAL, m_photoReal);
}

BOOL WinReg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadPicture(m_faceBG, IDB_PNG4);
	m_btnMouse.Init(IDB_PNG5, 4, BTN_TYPE_NORMAL);

	CString name;
	for (int i = 0; i < 16; i++)
	{
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP)LoadImage(0, name, 
					IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	SetTimer(1, 30, NULL);

	CFont font;
	font.CreatePointFont(150, L"YAHEI CONSOLAS HYBRID", NULL);
	m_name.SetFont(&font);

	::MoveWindow(m_snowBG.m_hWnd, 440, 0, 640, 609, 1);
	::MoveWindow(m_photoReal.m_hWnd, 440, 64, 640, 480, 1);
	
	srand(time(NULL));
	return 0;
}


BEGIN_MESSAGE_MAP(WinReg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REG, &WinReg::OnBnClickedButtonReg)
END_MESSAGE_MAP()


// WinReg 消息处理程序


void WinReg::OnPaint()
{
	drawPicOnPait(&m_faceBG, this, 0, 0);
}


void WinReg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int snowIdx = 0;
	static int cnt = 0;
	if (nIDEvent == 1) {
		m_snowBG.SetBitmap(m_snows[snowIdx]);
		snowIdx = (snowIdx + 1) % 16;
	}
	else if (nIDEvent == 2) {
		//使用摄像头捕获画面
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL);
		
		cnt++;
		if (cnt == 1)
		{
			mciSendString(L"play res/music/zhuche.mp3", 0, 0, 0);
		}
		else if (cnt == 30)
		{
			paiZhao(&cap, "pic/tmp/tmp.jpg");
			cnt = 0;
			KillTimer(2);//关闭定时器
			cap.release(); //关闭摄像头

			//需求
		//职业-颜值-用户名.jpg
		//使用人脸识别-计算颜值[计算相似度]


		//计算相似度--颜值
			IplImage* img1 = cvLoadImage("pic/tmp/tmp.jpg");
			IplImage* img2 = cvLoadImage("res/neijun.jpg");

			float ratio = faceCompare(faceModel, img1, img2);
			int score = ratio * 1000 + 80 + rand() % 15;

			if (score <= 0) {
				score = 60;
			}
			else if (score >= 100) {
				score = 99;
			}

			//生成职业
			const char* jobs[] = { "精神科主任", "精神科副主任", "精神科实习生" };


			//生成完整文件名
			//工作--用户名--颜值.jpg
			CString editName;
			m_name.GetWindowText(editName);
			USES_CONVERSION;
			char* editName2 = T2A(editName);


			char cmd[521];
			sprintf_s(cmd, sizeof(cmd), "move pic\\tmp\\tmp.jpg pic\\users\\%s-%s-%d.jpg",
				jobs[rand() % 3], editName2, score);

			system(cmd);//执行命令
			CDialogEx::OnOK(); //关闭窗口

			WinWelcome win;
			win.DoModal();
			//CDialogEx::OnOK(); //关闭窗口
		}
	}
	CDialogEx::OnTimer(nIDEvent);

}


void WinReg::OnBnClickedButtonReg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_name.GetWindowTextLength() == 0) {
	MessageBox(L"请输入用户名!");
	return;
	}
	//关闭定时器
	KillTimer(1);
	
	//准备打开摄像头
	//1.使用opencv处理摄像头
	//2.

	//更换墙纸
	HBITMAP pic = (HBITMAP)LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_snowBG.SetBitmap(pic);
	cap.open(0);
	SetTimer(2, 100, NULL);
	
}
