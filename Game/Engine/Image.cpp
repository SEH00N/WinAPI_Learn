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

bool Image::Load(wstring fileName)
{
	if (m_hBitmap)
		DeleteObject(m_hBitmap);

	m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (m_hBitmap == nullptr)
		return false;

	GetObject(m_hBitmap, sizeof(m_bitInfo), &m_bitInfo);
	m_width = m_bitInfo.bmWidth;
	m_height = m_bitInfo.bmHeight;

	return true;
}

bool Image::Load(HBITMAP hBitmap)
{
	if (hBitmap == nullptr)
		return false;

	if (m_hBitmap)
		DeleteObject(m_hBitmap);

	m_hBitmap = hBitmap;

	GetObject(m_hBitmap, sizeof(m_bitInfo), &m_bitInfo);
	m_width = m_bitInfo.bmWidth;
	m_height = m_bitInfo.bmHeight;

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
	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	DrawBitmap(hdc, x, y, dest_width, dest_height, 0, 0, m_width, m_height);
}

void Image::DrawBitmap(HDC hdc, int x, int y, int dest_width, int dest_height, int sx, int sy, int sw, int sh)
{
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_hBitmap);

	int centerX = x - dest_width / 2;
	int centerY= y - dest_height / 2;

	GdiTransparentBlt(hdc, centerX, centerY, dest_width, dest_height, hMemDC, sx, sy, sw, sh, m_transparentColor);

	DeleteDC(hMemDC);

}

void Image::DrawAlpha(HDC hdc, int x, int y, BYTE alpha, int dest_width, int dest_height)
{
	static HDC hAlphaMemDC;
	HBITMAP hAlphaBitmap, hOldBitmap;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height== 0)
		dest_height = m_height;

	if (!hAlphaMemDC)
		hAlphaMemDC = CreateCompatibleDC(hdc);

	int centerX = x - dest_width / 2;
	int centerY = y - dest_height / 2;
	hAlphaBitmap = CreateAlphaBitmap(hdc, centerX, centerY, dest_width, dest_height);

	hOldBitmap = (HBITMAP)SelectObject(hAlphaMemDC, hAlphaBitmap);

	m_blendFunc.SourceConstantAlpha = alpha;
	GdiAlphaBlend(hdc, centerX, centerY, dest_width, dest_height, hAlphaMemDC, 0, 0, dest_width, dest_height, m_blendFunc);

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

	hRotBitmap = CreateRotateBitmap(hdc, 0, 0, dest_width, dest_height, angle);
	hOldBitmap = (HBITMAP)SelectObject(hRotMemDC, hRotBitmap);

	int centerX = x - dest_width / 2;
	int centerY = y - dest_height / 2;

	GdiTransparentBlt(hdc, centerX, centerY, dest_width, dest_height, hRotMemDC, 0, 0, dest_width, dest_height, m_transparentColor);

	SelectObject(hRotMemDC, hOldBitmap);
	DeleteObject(hRotBitmap);
}
