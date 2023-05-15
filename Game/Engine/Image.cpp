#include "pch.h"
#include "Image.h"

Image::Image() : m_hBitmap(nullptr)
{
	memset(&m_bitInfo, 0, sizeof(m_bitInfo));
	m_width = 0;
	m_height = 0;
	m_transparentColor = RGB(255, 0, 255);

	m_angle = 0;
	m_rotate = FALSE;

	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;
	m_blendFunc.BlendOp = AC_SRC_OVER;
}

Image::~Image()
{
	if (m_hBitmap)
		DeleteObject(m_hBitmap);
}

bool Image::Load(wstring fileName, bool center)
{
	if (m_hBitmap)
		DeleteObject(m_hBitmap);

	m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (m_hBitmap == nullptr)
		return false;

	GetObject(m_hBitmap, sizeof(m_bitInfo), &m_bitInfo);
	m_width = m_bitInfo.bmWidth;
	m_height = m_bitInfo.bmHeight;

	m_center = center;

	return true;
}

bool Image::Load(HBITMAP hBitmap, bool center)
{
	if (hBitmap == nullptr)
		return false;

	if (m_hBitmap)
		DeleteObject(m_hBitmap);

	m_hBitmap = hBitmap;

	GetObject(m_hBitmap, sizeof(m_bitInfo), &m_bitInfo);
	m_width = m_bitInfo.bmWidth;
	m_height = m_bitInfo.bmHeight;
	
	m_center = center;

	return true;
}

bool Image::Release()
{
	if (m_hBitmap)
		DeleteObject(m_hBitmap);

	return true;
}

bool Image::IsEmpty() const
{
	return (m_hBitmap == nullptr);
}

HBITMAP Image::GetBitmapHandle() const
{
	return m_hBitmap;
}

const BITMAP& Image::GetBitmapInfo() const
{
	return m_bitInfo;
}

LONG Image::GetWidth() const
{
	return m_width;
}

LONG Image::GetHeight() const
{
	return m_height;
}

void Image::SetTransparentColor(COLORREF color)
{
	m_transparentColor = color;
}

COLORREF Image::GetTransparentColor() const
{
	return m_transparentColor;
}

RECT Image::GetBoundingBox()
{
	RECT rt;

	if (m_center)
	{
		rt.left = m_posX - m_destWidth / 2;
		rt.right = m_posX + m_destWidth / 2;
		rt.top = m_posY - m_destHeight / 2;
		rt.bottom = m_posY + m_destHeight / 2;
	}
	else
	{
		rt.left = m_posX;
		rt.right = rt.left + m_destWidth;
		rt.top = m_posY;
		rt.bottom = m_posY + m_destHeight;
	}

	return rt;
}

HBITMAP Image::CreateAlphaBitmap(HDC hdc, int x, int y, int dest_width, int dest_height)
{
	HDC sourceDC = CreateCompatibleDC(hdc);
	HDC destDC = CreateCompatibleDC(hdc);
	HBITMAP hResultBitmap = CreateCompatibleBitmap(hdc, dest_width, dest_height);

	HBITMAP hOldSource = (HBITMAP)SelectObject(sourceDC, m_hBitmap);
	HBITMAP hOldDest = (HBITMAP)SelectObject(destDC, hResultBitmap);

	BitBlt(destDC, 0, 0, dest_width, dest_height, hdc, x, y, SRCCOPY);
	GdiTransparentBlt(destDC, 0, 0, dest_width, dest_height, sourceDC, 0, 0, m_width, m_height, m_transparentColor);

	SelectObject(sourceDC, hOldSource);
	SelectObject(destDC, hOldDest);
	DeleteDC(sourceDC);
	DeleteDC(destDC);

	return hResultBitmap;
}

HBITMAP Image::CreateRotateBitmap(HDC hdc, int x, int y, int dest_width, int dest_height, FLOAT angle)
{
	HDC sourceDC = CreateCompatibleDC(hdc);
	HDC destDC = CreateCompatibleDC(hdc);

	HBITMAP hResultBitmap = CreateCompatibleBitmap(hdc, dest_width, dest_height);
	HBITMAP hOldSource = (HBITMAP)SelectObject(sourceDC, m_hBitmap);
	HBITMAP hOldDest = (HBITMAP)SelectObject(destDC, hResultBitmap);

	HBRUSH hbrBack = CreateSolidBrush(m_transparentColor);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);

	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY);

	DeleteObject(SelectObject(destDC, hbrOld));

	angle = (float)(angle * PI / 180.f);
	float cosine = cosf(angle);
	float sine = sinf(angle);

	SetGraphicsMode(destDC, GM_ADVANCED);

	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = sine;
	xform.eM21 = -sine;
	xform.eM22 = cosine;

	xform.eDx = (FLOAT)dest_width / 2;
	xform.eDy = (FLOAT)dest_height / 2;

	SetWorldTransform(destDC, &xform);
	GdiTransparentBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, 0, 0, m_width, m_height, m_transparentColor);

	SelectObject(sourceDC, hOldSource);
	SelectObject(destDC, hOldDest);

	DeleteDC(sourceDC);
	DeleteDC(destDC);

	return hResultBitmap;
}

void Image::DrawBitmap(HDC hdc, int x, int y, int dest_width, int dest_height)
{
	m_posX = x;
	m_posY = y;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	m_destWidth = dest_width;
	m_destHeight = dest_height;

	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_hBitmap);

	GdiTransparentBlt(hdc, x, y, dest_width, dest_height, hMemDC, 0, 0, m_width, m_height, m_transparentColor);

	DeleteDC(hMemDC);
}

void Image::DrawBitmap(HDC hdc, int x, int y, int dest_width, int dest_height, int sx, int sy, int sw, int sh)
{
	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_hBitmap);

	GdiTransparentBlt(hdc, x, y, dest_width, dest_height, hMemDC, sx, sy, sw, sh, m_transparentColor);

	DeleteDC(hMemDC);

}

void Image::DrawAlpha(HDC hdc, int x, int y, BYTE alpha, int dest_width, int dest_height)
{
	static HDC hAlphaMemDC;
	HBITMAP hAlphaBitmap, hOldBitmap;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	if (!hAlphaMemDC)
		hAlphaMemDC = CreateCompatibleDC(hdc);

	hAlphaBitmap = CreateAlphaBitmap(hdc, x, y, dest_width, dest_height);

	hOldBitmap = (HBITMAP)SelectObject(hAlphaMemDC, hAlphaBitmap);

	m_blendFunc.SourceConstantAlpha = alpha;
	GdiAlphaBlend(hdc, x, y, dest_width, dest_height, hAlphaMemDC, 0, 0, dest_width, dest_height, m_blendFunc);

	SelectObject(hAlphaMemDC, hOldBitmap);
	DeleteObject(hAlphaBitmap);
}

void Image::DrawRotate(HDC hdc, int x, int y, float angle, int dest_width, int dest_height)
{
	static HDC hRotMemDC;
	HBITMAP hRotBitmap, hOldBitmap;

	if(!hRotMemDC)
		hRotMemDC = CreateCompatibleDC(hdc);
	 
	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	hRotBitmap = CreateRotateBitmap(hdc, 0, 0, dest_width, dest_height, angle);
	hOldBitmap = (HBITMAP)SelectObject(hRotMemDC, hRotBitmap);

	GdiTransparentBlt(hdc, x, y, dest_width, dest_height, hRotMemDC, 0, 0, dest_width, dest_height, m_transparentColor);

	SelectObject(hRotMemDC, hOldBitmap);
	DeleteObject(hRotBitmap);
}

void Image::DrawLoopBitmap(HDC hdc, const LPRECT rtDraw, int offsetX, int offsetY)
{
	if (offsetX < 0)
		offsetX = m_width + (offsetX % m_width);
	if (offsetY < 0)
		offsetY = m_height + (offsetY % m_height);

	RECT rtSource;
	int sourceWidth;
	int sourceHeight;

	if (rtDraw == nullptr)
		return;

	RECT rtDest;
	int drawAreaX = rtDraw->left;
	int drawAreaY = rtDraw->top;
	int drawWidth = rtDraw->right - rtDraw->left;
	int drawHeight = rtDraw->bottom - rtDraw->top;

	for (int y = 0; y < drawHeight; y += sourceHeight)
	{
		rtSource.top = (y + offsetY) % m_height;
		rtSource.bottom = m_height;

		sourceHeight = rtSource.bottom - rtSource.top;

		if (y + sourceHeight > drawHeight)
		{
			rtSource.bottom -= (y + sourceHeight) - drawHeight;
			sourceHeight = rtSource.bottom - rtSource.top;
		}

		rtDest.top = y + drawAreaY;
		rtDest.bottom = rtDest.top + sourceHeight;
	
		for (int x = 0; x < drawWidth; x += sourceWidth)
		{
			rtSource.left = (x + offsetX) % m_width;
			rtSource.right = m_width;

			sourceWidth = rtSource.right - rtSource.left;

			if (x + sourceWidth > drawWidth)
			{
				rtSource.right -= (x + sourceWidth) - drawWidth;
				sourceWidth = rtSource.right - rtSource.left;
			}

			rtDest.left = x + drawAreaX;
			rtDest.right = rtDest.left + sourceWidth;

			int width = rtSource.right - rtSource.left;
			int height = rtSource.bottom - rtSource.top;

			DrawBitmap(hdc, rtSource.left, rtSource.top, width, height, rtSource.left, rtSource.top, width, height);
		}
	}
}
