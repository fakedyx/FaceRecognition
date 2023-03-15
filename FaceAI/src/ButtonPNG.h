#pragma once

#include "pch.h"
#include <atlimage.h>

#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif 
//#include "Includes/GdiPlus.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")


//��ť��״̬
enum
{
	CTRL_NOFOCUS = 0x01,			//��ͨ
	CTRL_FOCUS,						//mousemove
	CTRL_SELECTED,					//buttondown
	CTRL_DISABLE,					//��Ч
};

//ͼƬ��ʽ
enum
{
	BTN_IMG_1 = 1,					//
	BTN_IMG_3 = 3,					//3��ͼ��1��ͼƬ����3Сͼ����ͬ��
	BTN_IMG_4 = 4,					//4��ͼ
};

//��ť����
enum
{
	BTN_TYPE_NORMAL = 0x10,			//��ͨBTN
	BTN_TYPE_MENU,					//�˵����͵�BTN
	BTN_TYPE_STATIC,				//��̬���͵�BTN
};

//����Դ�������λͼ
bool LoadImageFromResourse(CImage* pImg, UINT nImgID, LPCTSTR lpImgType);
bool LoadPicture(CImage& bmp, UINT nImgID, LPCTSTR lpImgType = _T("PNG"));			//��Alphaͨ����ͼƬ�����CImage

void CreateStretchImage(CImage* pImage, CImage* ResultImage, int StretchWidth, int StretchHeight);

#if _MSC_VER > 1000
#pragma once
#endif 

class ButtonPNG : public CButton
{
public:
	ButtonPNG();
	virtual ~ButtonPNG();

public:
	void Init(UINT nImg, int nPartNum, UINT nBtnType=BTN_TYPE_NORMAL);
	bool ShowImage(CDC* pDC, Image* pImage, UINT nState);
	Image *ImageFromResource(HINSTANCE hInstance,UINT uImgID,LPCTSTR lpType);
	void PaintParent();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

public:
	bool m_bTracked;

	UINT m_nState;

private:
	int m_nImgPart;

	Image* m_pImage;


	UINT m_nBtnType;
	BOOL m_bMenuOn;		//BTN����ΪBTN_TYPE_MENUʱ���Ƿ��ڰ��µ�״̬
};

void drawPicOnPait(CImage* img, CWnd* wnd, int x, int y);

//#endif 
