// WinLogin.cpp: 实现文件
//

#include "pch.h"
#include "FaceAi.h"
#include "afxdialogex.h"
#include "WinLogin.h"
#include <opencv2/opencv.hpp>
#include <mmsystem.h>
#include "faceTool.h"
#include <vector>
using namespace std;
using namespace cv;
#include "tools.h"
#pragma comment(lib, "winmm.lib")

static VideoCapture cap; //定义摄像头
extern MHandle faceModel;


// WinLogin 对话框

IMPLEMENT_DYNAMIC(WinLogin, CDialogEx)

WinLogin::WinLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACEAI_LOG, pParent)
{

}

WinLogin::~WinLogin()
{
}

BOOL WinLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//再初始化自己的
	LoadPicture(m_imgBG, IDB_PNG4);
	m_btnMouse.Init(IDB_PNG5, 4, BTN_TYPE_NORMAL);

	//char name[256];
	CString name;
	//加载雪花
	for (int i = 0; i < 16; i++)
	{
		//sprintf(name, "res/snow/snow_%d.bmp", i);
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP)LoadImage(0, name,
						IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	//启动定时器
	SetTimer(1, 30, NULL);



	return 0;
}

void WinLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_LOG, m_btnMouse);
	DDX_Control(pDX, IDC_SNOW, m_snow);
	DDX_Control(pDX, IDC_PHOTO_REAL, m_photo);
}


BEGIN_MESSAGE_MAP(WinLogin, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &WinLogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_LOG, &WinLogin::OnBnClickedButtonLog)
END_MESSAGE_MAP()


// WinLogin 消息处理程序


void WinLogin::OnPaint()
{
	drawPicOnPait(&m_imgBG, this, 0, 0);
}

bool faceCheck(const char* fileName, char* userName)
{
	//抓拍照片与存储照片实现一一对比
	vector<CString> vctPath;

	WIN32_FIND_DATA fileData;
	HANDLE file = FindFirstFile(L"pic/users/*.jpg", &fileData);
	if (file != INVALID_HANDLE_VALUE) {
		//找到
		vctPath.push_back(fileData.cFileName);
		while (FindNextFile(file, &fileData)) {
			vctPath.push_back(fileData.cFileName);
		}
	}

	IplImage* img1 = cvLoadImage(fileName);
	IplImage* img2;
	char name[256];
	bool isReg = false;
	for (int i = 0; i < vctPath.size(); i++) {
		USES_CONVERSION;
		char* otherName = T2A(vctPath[i]);
		sprintf_s(name, sizeof(name), "pic/users/%s", otherName);
		img2 = cvLoadImage(name);
		float ret = faceCompare(faceModel, img1, img2);

		/*相似度*/
		if (ret >= 0.92)
		{
			/*找到了*/
			isReg = true;
			string tmpStr = otherName;
			string nameStr = tmpStr.substr(0, tmpStr.length() - 4);  //.jpg不要
			strcpy(userName, nameStr.c_str());
		}
	}
	return isReg;
}

void WinLogin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int snowIndex = 0;
	static int cnt = 0;
	static char userName[256] = "";
	static bool success = false;

	if (nIDEvent == 1) //1号定时器
	{	
		m_snow.SetBitmap(m_snows[snowIndex]);
		snowIndex = (snowIndex + 1) % 16;
	}
	else if (nIDEvent == 2)
	{
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL);

		cnt++;
		if (cnt == 10)
		{
			const char* fileName = "pic/tmp/tmp.jpg";
			paiZhao(&cap, fileName);

			//识别
			if (faceCheck(fileName, userName)) {
				//播放登录声音
				mciSendString(L"play res/music/login.mp3", 0, 0, 0);
				success = true;
			}
		}
		else if(cnt == 20) {
			cnt = 0;
			KillTimer(2);
			cap.release();

			if (success) 
			{
				strcpy(m_job, strtok(userName, "-"));
				strcpy(m_username, strtok(NULL, "-"));
				m_score = atoi(strtok(NULL, "."));
				m_logined = true;

				CDialogEx::OnOK();
			}
			else {
				m_logined = false;
				MessageBox(L"登录失败!");
				SetTimer(1, 100, NULL);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void WinLogin::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void WinLogin::OnBnClickedButtonLog()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);

	//更换墙纸
	HBITMAP pic = (HBITMAP)LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_snow.SetBitmap(pic);

	cap.open(0);

	::MoveWindow(m_photo.m_hWnd, 440, 64, 640, 480, 1);
	SetTimer(2, 100, NULL);


	
}
