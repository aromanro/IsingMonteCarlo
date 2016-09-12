#include "stdafx.h"
#include "MemoryBitmap.h"
#include "SpinMatrix.h"

#include "IsingMonteCarlo.h"
#include "Options.h"

#include <cassert>

MemoryBitmap::MemoryBitmap()
	: m_width(0), m_height(0), data(nullptr)
{	
}


MemoryBitmap::~MemoryBitmap()
{
	delete[] data;
}


void MemoryBitmap::SetSize(int width, int height)
{
	assert(width != 0 && height != 0);

	if (m_width != width || m_height != height)
	{
		delete[] data;

		m_width = width;
		m_height = height;

		data = new unsigned char[GetStrideLength() * height];
	}
}

void MemoryBitmap::SetMatrix(MonteCarlo::SpinMatrix& matrix, unsigned int spinsWidth, unsigned int spinsHeight, unsigned int spinSize)
{
	SetSize(spinSize*spinsWidth, spinSize*spinsHeight);

	int widthZeroOrig = (spinsWidth - (int)matrix.getCols()) / 2;
	int heightZeroOrig = (spinsHeight - (int)matrix.getRows()) / 2;

	for (unsigned int i = 0; i < spinsHeight; ++i)
		for (unsigned int j = 0; j < spinsWidth; ++j)
		{
			int spin = matrix.GetSpinPeriodic(i - heightZeroOrig, j - widthZeroOrig);

			FillSquare(j * spinSize, i * spinSize, spinSize, spin > 0 ? theApp.options.spinUpColor : theApp.options.spinDownColor);
		}
}


void MemoryBitmap::Draw(CDC* pDC)
{
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFOHEADER));
	
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = m_height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CBitmap bitmap;
	
	bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	::SetDIBits(pDC->GetSafeHdc(), bitmap, 0, m_height, data, &bmi, DIB_RGB_COLORS);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap = dcMemory.SelectObject(&bitmap);
	pDC->BitBlt(0, 0, m_width, m_height, &dcMemory, 0, 0, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
}

void MemoryBitmap::Draw(CDC* pDC, CRect& rect, int origWidth, int origHeight)
{
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = m_height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CBitmap bitmap;

	bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	::SetDIBits(pDC->GetSafeHdc(), bitmap, 0, m_height, data, &bmi, DIB_RGB_COLORS);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap = dcMemory.SelectObject(&bitmap);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, origWidth ? (m_width - origWidth)/2 : 0, origHeight ? (m_height - origHeight)/2 : 0, origWidth ? origWidth : m_width, origHeight ? origHeight : m_height, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
}




