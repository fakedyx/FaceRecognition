#include "pch.h"
#include "tools.h"
#include <opencv2/opencv.hpp>
#include "CvvImage.h"
#include <Windows.h>

using namespace cv;

void paiZhao(VideoCapture* cap, const char* name)
{
	CvvImage out; //CvvImage, opencv中用来表示图片的类
	CRect outrect;
	if (!cap->isOpened())
	{
		MessageBox(NULL, _T("摄像头打开失败！"), _T("警告"), MB_OK);
		return;
	}

	Mat frame; //Mat 用来表示图片，是C++类
	*cap >> frame;
	IplImage outImage = frame;

	//char  fileName[64];
	//sprintf(fileName, "%s.jpg", name);
	//getCamera(&cap);
	cvSaveImage(name, &outImage, 0);//保存图片
}

void showImage(CvvImage* out, HWND hwnd, UINT imgID) {
	CRect outrect;

	int x = out->Width(); //电脑摄像头的默认大小 640*480
	int y = out->Height();
	//获取Picture Control控件的大小  
	CWnd* pCWnd = CWnd::FromHandle(hwnd);
	pCWnd->GetDlgItem(imgID)->GetWindowRect(&outrect);
	//将客户区选中到控件表示的矩形区域内  
	pCWnd->ScreenToClient(&outrect);
	//窗口移动到控件表示的区域  
	pCWnd->GetDlgItem(imgID)->MoveWindow(outrect.left, outrect.top, x, y, TRUE);
	CWnd* ppWnd = NULL;
	ppWnd = pCWnd->GetDlgItem(imgID);//获取控件句柄  
	ppWnd->GetClientRect(&outrect);//获取句柄指向控件区域的大小  
	CDC* ppDc = NULL;
	ppDc = ppWnd->GetDC();//获取picture的DC  
	out->DrawToHDC(ppDc->m_hDC, outrect);//将图片绘制到picture表示的区域内  
	pCWnd->ReleaseDC(ppDc);
}

void refreshCamera(VideoCapture* cap, HWND hwnd, UINT id)
{
	CvvImage out; //CvvImage, opencv中用来表示图片的类
	CRect outrect;
	if (!cap->isOpened())
	{
		MessageBox(NULL, _T("摄像头打开失败！"), _T("警告"), MB_OK);
		return;
	}

	Mat frame; //Mat 用来表示图片，是C++类
	*cap >> frame;
	IplImage outImage = frame;
	//outvedio = &outImage;
	out.CopyOf(&outImage);

	showImage(&out, hwnd, id);
}

// draw srcImage to destImage
void putimagePNG(int  picture_x, int picture_y, IMAGE* srcImage, IMAGE* destImage) //x为载入图片的X坐标，y为Y坐标
{
	SetWorkingImage(destImage);
	DWORD* dst = GetImageBuffer(destImage);    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer(destImage);
	DWORD* src = GetImageBuffer(srcImage); //获取picture的显存指针
	int picture_width = srcImage->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = srcImage->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
	SetWorkingImage();
}
