#include "pch.h"
#include "Image.h"

Image::Image() : m_hBitmap(nullptr)
{
	memset(&m_bitInfo, 0, sizeof(m_bitInfo));
	m_width = 0;
	m_height = 0;
	m_transColor = RGB(255, 0, 255);
	m_angle = 0;
	m_rotate = FALSE;
	m_center = TRUE;

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
	return m_hBitmap == nullptr;
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

void Image::SetTransColor(COLORREF color)
{
	m_transColor = color;
}

COLORREF Image::GetTransColor() const
{
	return m_transColor;
}

RECT Image::GetBoundingBox()
{
	RECT rc;
	if (m_center)
	{
		rc.left = m_posX - m_dest_width / 2;
		rc.right = rc.left + m_dest_width;
		rc.top = m_posY - m_dest_height / 2;
		rc.bottom = rc.top + m_dest_height;
	}
	else
	{
		rc.left = m_posX;
		rc.right = rc.left + m_dest_width;
		rc.top = m_posY;
		rc.bottom = rc.top + m_dest_height;
	}
	return rc;
}

HBITMAP Image::CreateAlphaBitmap(HDC hdc, int x, int y, int dest_width, int dest_height)
{
	HDC sourceDC = CreateCompatibleDC(hdc);		// �̹��� DC
	HDC destDC = CreateCompatibleDC(hdc);		// ���� DC
	HBITMAP hResultBitmap = CreateCompatibleBitmap(hdc, dest_width, dest_height);

	HBITMAP hOldSource = (HBITMAP)SelectObject(sourceDC, m_hBitmap);
	HBITMAP hOldDest = (HBITMAP)SelectObject(destDC, hResultBitmap);

	BitBlt(destDC, 0, 0, dest_width, dest_height, hdc, x, y, SRCCOPY);

	GdiTransparentBlt(
		destDC, 0, 0, dest_width, dest_height,
		sourceDC, 0, 0, m_width, m_height, m_transColor);

	SelectObject(sourceDC, hOldSource);
	SelectObject(destDC, hOldDest);
	DeleteDC(sourceDC);
	DeleteDC(destDC);

	return hResultBitmap;
}

HBITMAP Image::CreateRotateBitmap(HDC hdc, int x, int y, int dest_width, int dest_height, float angle)
{
	// ���� �̹��� DC
	HDC sourceDC = CreateCompatibleDC(hdc);
	// ȸ�� �̹��� DC
	HDC destDC = CreateCompatibleDC(hdc);
	// ȸ�� �̹��� ��Ʈ��
	HBITMAP hResultBitmap = CreateCompatibleBitmap(hdc, dest_width, dest_height);

	HBITMAP hOldSource = (HBITMAP)SelectObject(sourceDC, m_hBitmap);
	HBITMAP hOldDest = (HBITMAP)SelectObject(destDC, hResultBitmap);

	HBRUSH hbrBack = CreateSolidBrush(m_transColor);
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);
	// �귯�� �÷��� ����� ŭ ä����
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY);
	DeleteObject(SelectObject(destDC, hbrOld));

	// ȣ���� ��ȯ
	angle = (float)(angle * PI / 180);
	float cosine = cosf(angle);
	float sine = sinf(angle);

	// ���� ��ȯ�� ���� �׷��� ��� ��ȯ : Ȯ����
	SetGraphicsMode(destDC, GM_ADVANCED);

	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = sine;
	xform.eM21 = -sine;
	xform.eM22 = cosine;
	xform.eDx = (FLOAT)dest_width / 2;
	xform.eDy = (FLOAT)dest_height / 2;

	SetWorldTransform(destDC, &xform);

	GdiTransparentBlt(
		destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height,
		sourceDC, 0, 0, m_width, m_height, m_transColor);

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

	if (m_center == TRUE)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	m_dest_width = dest_width;
	m_dest_height = dest_height;

	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_hBitmap);

	GdiTransparentBlt(
		hdc, x, y, dest_width, dest_height,
		hMemDC, 0, 0, m_width, m_height, m_transColor);

	DeleteDC(hMemDC);
}

void Image::DrawBitmap(HDC hdc, int x, int y, int dest_width, int dest_height, int sx, int sy, int sw, int sh)
{
	m_posX = x;
	m_posY = y;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center == TRUE)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	m_dest_width = dest_width;
	m_dest_height = dest_height;

	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, m_hBitmap);

	GdiTransparentBlt(
		hdc, x, y, dest_width, dest_height,
		hMemDC, sx, sy, sw, sh, m_transColor);

	DeleteDC(hMemDC);
}

void Image::DrawAlpha(HDC hdc, int x, int y, BYTE alpha, int dest_width, int dest_height)
{
	m_posX = x;
	m_posY = y;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center == TRUE)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	m_dest_width = dest_width;
	m_dest_height = dest_height;

	static HDC hAlphaMemDC;
	HBITMAP hAlphaBitmap, hOldBitmap;


	if (!hAlphaMemDC)
		hAlphaMemDC = CreateCompatibleDC(hdc);

	hAlphaBitmap = CreateAlphaBitmap(hdc, x, y, dest_width, dest_height);

	hOldBitmap = (HBITMAP)SelectObject(hAlphaMemDC, hAlphaBitmap);

	m_blendFunc.SourceConstantAlpha = alpha;
	GdiAlphaBlend(
		hdc, x, y, dest_width, dest_height,
		hAlphaMemDC, 0, 0, dest_width, dest_height, m_blendFunc);

	SelectObject(hAlphaMemDC, hOldBitmap);
	DeleteObject(hAlphaBitmap);
}

void Image::DrawRotate(HDC hdc, int x, int y, float angle, int dest_width, int dest_height)
{
	m_posX = x;
	m_posY = y;

	if (dest_width == 0)
		dest_width = m_width;
	if (dest_height == 0)
		dest_height = m_height;

	if (m_center == TRUE)
	{
		x = x - dest_width / 2;
		y = y - dest_height / 2;
	}

	m_dest_width = dest_width;
	m_dest_height = dest_height;

	static HDC hRotMemDC;
	HBITMAP hRotBitmap, hOldBitmap;

	if( !hRotMemDC )
		hRotMemDC = CreateCompatibleDC(hdc);

	// ȸ���� �̹��� ����
	hRotBitmap = CreateRotateBitmap(hdc, 0, 0, dest_width, dest_height, angle);

	// ȸ�� �� �̹��� memDC�� ��� �ش�.
	hOldBitmap = (HBITMAP)SelectObject(hRotMemDC, hRotBitmap);

	GdiTransparentBlt(
		hdc, x, y, dest_width, dest_height,
		hRotMemDC, 0, 0, dest_width, dest_height, m_transColor);

	SelectObject(hRotMemDC, hOldBitmap);
	DeleteObject(hRotBitmap);
}

void Image::DrawLoopBitmap(HDC hdc, const LPRECT rtDraw, int offsetX, int offsetY)
{
	if (offsetX < 0)
		offsetX = m_width + (offsetX % m_width);
	if (offsetY < 0)
		offsetY = m_height + (offsetY % m_height);

	// �׷����� ����(�̹���) ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	// �׷����� DC ���� ����
	if (rtDraw == nullptr)
		return;

	RECT rcDest;
	int drawAreaX = rtDraw->left;
	int drawAreaY = rtDraw->top;
	int drawWidth = rtDraw->right - rtDraw->left;
	int drawHeight = rtDraw->bottom - rtDraw->top;

	// ���� ���� ���� ���ϱ�
	for (int y = 0; y < drawHeight; y += sourHeight)
	{
		// ������ ���� ���
		rcSour.top = (y + offsetY) % m_height;
		rcSour.bottom = m_height;
		sourHeight = rcSour.bottom - rcSour.top;

		// sour������ �׸��� ȭ���� �Ѿ� ���� ��
		if (y + sourHeight > drawHeight)
		{
			rcSour.bottom -= (y + sourHeight) - drawHeight;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		// �׷����� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		// ���� ���� ����
		for (int x = 0; x < drawWidth; x += sourWidth)
		{
			rcSour.left = (x + offsetX) % m_width;
			rcSour.right = m_width;
			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawWidth)
			{
				rcSour.right -= (x + sourWidth) - drawWidth;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;
			int width = rcSour.right - rcSour.left;
			int height = rcSour.bottom - rcSour.top;

			// �׸��� 
			DrawBitmap(hdc, rcDest.left, rcDest.top, width, height, rcSour.left, rcSour.top, width, height);
		}
	}
}
