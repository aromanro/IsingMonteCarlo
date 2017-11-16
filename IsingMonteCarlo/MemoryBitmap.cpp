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


MemoryBitmap::MemoryBitmap(const MemoryBitmap& other) // copy constructor
{
	if (other.data)
	{
		m_width = other.m_width;
		m_height = other.m_height;
		int size = GetStrideLength() * m_height;
		data = new unsigned char[size];
		memcpy(data, other.data, size);
	}
	else {
		data = nullptr;
		m_width = m_height = 0;
	}
}

MemoryBitmap::MemoryBitmap(MemoryBitmap&& other) noexcept // move constructor
	: data(other.data), m_width(other.m_width), m_height(other.m_height)
{
	other.data = nullptr;
	other.m_height = other.m_width = 0;
}

MemoryBitmap& MemoryBitmap::operator=(const MemoryBitmap& other) //copy assignment operator
{
	MemoryBitmap temp(other);
	
	*this = std::move(temp);

	return *this;
}

MemoryBitmap& MemoryBitmap::operator=(MemoryBitmap&& other) noexcept // move assignment operator
{
	delete[] data;
	
	m_width = other.m_width;
	m_height = other.m_height;

	data = other.data;

	other.m_height = other.m_width = 0;
	other.data = nullptr;

	return *this;
}

MemoryBitmap::~MemoryBitmap() noexcept
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

void MemoryBitmap::SetMatrix(const MonteCarlo::SpinMatrix& matrix, unsigned int spinsWidth, unsigned int spinsHeight, unsigned int spinSize)
{
	SetSize(spinSize*spinsWidth, spinSize*spinsHeight);

	const int widthZeroOrig = (spinsWidth - static_cast<int>(matrix.getCols())) / 2;
	const int heightZeroOrig = (spinsHeight - static_cast<int>(matrix.getRows())) / 2;

	for (unsigned int i = 0; i < spinsHeight; ++i)
		for (unsigned int j = 0; j < spinsWidth; ++j)
		{
			const int spin = matrix.GetSpinPeriodic(i - heightZeroOrig, j - widthZeroOrig);

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




