// PngButton.cpp : implementation file
//

//#include "stdafx.h"
#include "pch.h"
#include "ButtonPNG.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPngButton

ButtonPNG::ButtonPNG()
{
	m_bTracked=false;
 	m_bMenuOn = FALSE;

	m_nImgPart = 0;
	m_pImage = NULL;

	m_nState = CTRL_NOFOCUS;
	m_nBtnType = BTN_TYPE_NORMAL;

}

ButtonPNG::~ButtonPNG()
{
	if(m_pImage == NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

void ButtonPNG::Init(UINT nImg, int nPartNum, UINT nBtnType)
{
	m_pImage = ImageFromResource(AfxGetResourceHandle(), nImg, L"PNG");
	m_nBtnType = nBtnType;
	m_nImgPart = nPartNum;

	if (m_pImage == NULL)
		return;
	
	CRect rcButton;

	if (m_nImgPart == BTN_IMG_1)
		rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	else if(m_nImgPart == BTN_IMG_3)
		rcButton = CRect(0, 0, m_pImage->GetWidth()/3, m_pImage->GetHeight());
	else if (m_nImgPart == BTN_IMG_4)
		rcButton = CRect(0, 0, m_pImage->GetWidth()/4, m_pImage->GetHeight());
	else
		return;

	SetWindowPos(NULL, 0, 0, rcButton.Width(), rcButton.Height(), SWP_NOACTIVATE|SWP_NOMOVE);
}

BEGIN_MESSAGE_MAP(ButtonPNG, CButton)
	//{{AFX_MSG_MAP(CPngButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHOver)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPngButton message handlers

void ButtonPNG::OnPaint()
{
	CButton::OnPaint();
}

void ButtonPNG::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 	if (!IsWindowEnabled())
 		m_nState = CTRL_DISABLE;
 
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

 	ShowImage(&dc, m_pImage, m_nState);
}

bool ButtonPNG::ShowImage(CDC* pDC, Image* pImage, UINT nState)
{
	bool bSuc = false;

	if(pImage!=NULL)
	{
		CRect rcButton;

		if (m_nImgPart == BTN_IMG_1)
			rcButton = CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
		else if(m_nImgPart == BTN_IMG_3)
		{
			if (nState == CTRL_NOFOCUS)
				rcButton = CRect(0, 0, m_pImage->GetWidth()/3, m_pImage->GetHeight());
			else if(nState == CTRL_FOCUS)
				rcButton = CRect(m_pImage->GetWidth()/3, 0, m_pImage->GetWidth()/3 * 2, m_pImage->GetHeight());
			else if (nState == CTRL_SELECTED)
				rcButton = CRect(m_pImage->GetWidth()/3 * 2, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
			else
				return false;
		}
		else if (m_nImgPart == BTN_IMG_4)
		{
			if (nState == CTRL_NOFOCUS)
				rcButton = CRect(0, 0, m_pImage->GetWidth()/4, m_pImage->GetHeight());
			else if(nState == CTRL_FOCUS)
				rcButton = CRect(m_pImage->GetWidth()/4, 0, m_pImage->GetWidth()/4 * 2, m_pImage->GetHeight());
			else if (nState == CTRL_SELECTED)
				rcButton = CRect(m_pImage->GetWidth()/4 * 2, 0, m_pImage->GetWidth()/4 * 3, m_pImage->GetHeight());
			else if (nState == CTRL_DISABLE)
				rcButton = CRect(m_pImage->GetWidth()/4 * 3, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
			else
				return false;
		}
		else
			return false;

		Graphics graph(pDC->GetSafeHdc());
		graph.DrawImage(pImage, 0, 0, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), UnitPixel);
		graph.ReleaseHDC(pDC->GetSafeHdc());
		bSuc=true;
	}

	return bSuc;
}

Image *ButtonPNG::ImageFromResource(HINSTANCE hInstance,UINT uImgID,LPCTSTR lpType)
{
    HRSRC hResInfo=::FindResource(hInstance,MAKEINTRESOURCE(uImgID),lpType);
	if(hResInfo==NULL)
		return NULL; //fail
	DWORD dwSize;
	dwSize=SizeofResource(hInstance,hResInfo); //get resource size(bytes) 
	HGLOBAL hResData;
	hResData=::LoadResource(hInstance,hResInfo);
	if(hResData==NULL)
		return NULL; //fail
    HGLOBAL hMem;
	hMem=::GlobalAlloc(GMEM_MOVEABLE,dwSize);
    if(hMem==NULL){
		::FreeResource(hResData);
		return NULL;
	}
	LPVOID lpResData,lpMem;
	lpResData=::LockResource(hResData);
	lpMem=::GlobalLock(hMem);
	::CopyMemory(lpMem,lpResData,dwSize); //copy memory
	::GlobalUnlock(hMem);
    ::FreeResource(hResData); //free memory
    
	IStream *pStream;
	HRESULT hr;
	hr=::CreateStreamOnHGlobal(hMem,TRUE,&pStream);//create stream object
	Image *pImage=NULL;
	if(SUCCEEDED(hr)){
		pImage=Image::FromStream(pStream);//get GDI+ pointer
		pStream->Release();
	}
	::GlobalFree(hMem);
	return pImage;
}

void ButtonPNG::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);

	if (NULL != GetSafeHwnd())
	{
		if (!(GetButtonStyle() & WS_CLIPSIBLINGS))
			SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(GetSafeHwnd(),
			GWL_STYLE) | WS_CLIPSIBLINGS);
	}

	CButton::PreSubclassWindow();
}

BOOL ButtonPNG::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void ButtonPNG::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bTracked){
		TRACKMOUSEEVENT tme;
		ZeroMemory(&tme,sizeof(TRACKMOUSEEVENT));
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_HOVER|TME_LEAVE;
		tme.dwHoverTime=1;
		tme.hwndTrack=this->GetSafeHwnd();
		if(::_TrackMouseEvent(&tme))
			m_bTracked=true;
	}

	CButton::OnMouseMove(nFlags, point);
}

void ButtonPNG::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nState != CTRL_SELECTED)
	{
		m_nState = CTRL_SELECTED;
		
		if (!m_bMenuOn)
			m_bMenuOn = TRUE;

		PaintParent();
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

void ButtonPNG::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nState != CTRL_FOCUS)
	{
		m_nState = CTRL_FOCUS;
		PaintParent();
	}
	
	CButton::OnLButtonUp(nFlags, point);
}

LRESULT ButtonPNG::OnMouseHOver(WPARAM wParam,LPARAM lParam)
{
    //鼠标放上去时
	if (m_nState != CTRL_FOCUS)
	{
		m_nState = CTRL_FOCUS;
		PaintParent();	
	}
	
	return 0;
}
LRESULT ButtonPNG::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
    //鼠标移开时
	m_bTracked=false;

	if (m_nBtnType == BTN_TYPE_NORMAL)
		m_nState = CTRL_NOFOCUS;
	else if (m_nBtnType == BTN_TYPE_MENU)
	{
		if (m_bMenuOn)
			m_nState = CTRL_SELECTED;
		else
			m_nState = CTRL_NOFOCUS;
	}

	PaintParent(); 
	

	return 0;
}

void ButtonPNG::PaintParent() 
{   
	CRect   rect;  
	GetWindowRect(&rect);  
	GetParent()-> ScreenToClient(&rect);  
	GetParent()-> InvalidateRect(&rect); 
} 


bool LoadImageFromResourse(CImage* pImg, UINT nImgID, LPCTSTR lpImgType)
{
	if (pImg == NULL)
	{
		return FALSE;
	}
	pImg->Destroy();

	//查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nImgID), lpImgType);
	if (hRsrc == NULL)
	{
		return false;
	}

	//加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}


	LPVOID lpVoid = ::LockResource(hImgData);							//锁定内存中指定资源
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	::GlobalUnlock(hNew);									           //解除资源锁定

	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if (ht != S_OK)
	{
		GlobalFree(hNew);
	}
	else
	{
		//加载图片
		pImg->Load(pStream);
		GlobalFree(hNew);
	}

	//释放资源
	::FreeResource(hImgData);
	return true;
}

bool LoadPicture(CImage& bmp, UINT nImgID, LPCTSTR lpImgType)			//含Alpha通道的图片处理成CImage
{
	LoadImageFromResourse(&bmp, nImgID, lpImgType);					//加载图片资源

	if (bmp.IsNull())
	{
		return false;
	}
	if (bmp.GetBPP() == 32)												//确认该图片包含Alpha通道
	{
		for (int i = 0; i < bmp.GetWidth(); i++)
		{
			for (int j = 0; j < bmp.GetHeight(); j++)
			{
				byte* pByte = (byte*)bmp.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	return true;
}

void drawPicOnPait(CImage* img, CWnd* wnd, int x, int y) {
	CPaintDC dc(wnd);
	CDC* pDC = &dc;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CRect rcClient;
	GetClientRect(wnd->m_hWnd, &rcClient);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, img->GetWidth(), img->GetHeight());
	dcMem.SelectObject(memBitmap);

	dcMem.FillSolidRect(rcClient, RGB(255, 255, 255));	//设置画布颜色

	if (!img->IsNull())
	{
		//CRect rcImg = CRect(x, y, img->GetWidth(), img->GetHeight());
		CRect rcImg = CRect(0, 0, img->GetWidth(), img->GetHeight());
		img->Draw(dcMem.m_hDC, rcImg, rcImg);
	}

	pDC->BitBlt(x, y, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
}

void CreateStretchImage(CImage* pImage, CImage* ResultImage, int StretchWidth, int StretchHeight)
{
	if (pImage->IsDIBSection())
	{
		//取得pImage的DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC());//Image因为有自己的DC，所以必须使用FromHandle取得对应的DC
		CBitmap* bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);
		//建立新的CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());
		//当Destination比较小的时候，会根据Destination DC上的Stretch Blt mode决定是否保留删除点的资讯
		ResultImageDC->SetStretchBltMode(HALFTONE);//使用高品质
		::SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL);//调整Brush的起点
		//把pImage画到ResultImage上面
		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}