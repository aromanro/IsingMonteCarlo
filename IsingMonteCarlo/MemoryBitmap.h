#pragma once

namespace MonteCarlo {
	class SpinMatrix;
}

class MemoryBitmap
{
public:
	MemoryBitmap();

	MemoryBitmap(const MemoryBitmap& other); // copy constructor
	MemoryBitmap(MemoryBitmap&& other) noexcept; // move constructor
	MemoryBitmap& operator=(const MemoryBitmap& other); //copy assignment operator
	MemoryBitmap& operator=(MemoryBitmap&& other) noexcept; // move assignment operator

	~MemoryBitmap() noexcept;



	void SetSize(int width, int height);

	inline void FillSquare(int Xpos, int Ypos, int size, COLORREF color)
	{
		if (!data) return;

		const int stride = GetStrideLength();

		const int limX = 3 * min(Xpos + size, m_width);
		const int limY = min(Ypos + size, m_height);

		for (int line = Ypos; line < limY; ++line)
		{
			const int startLine = (m_height - line - 1) * stride;

			for (int horz = 3 * Xpos; horz < limX; horz += 3)
			{
				const int pos = startLine + horz;

				data[pos] = GetBValue(color);
				data[pos + 1] = GetGValue(color);
				data[pos + 2] = GetRValue(color);
			}
		}
	}


	void SetMatrix(const MonteCarlo::SpinMatrix& matrix, unsigned int spinsWidth, unsigned int spinsHeight, unsigned int spinSize);

	void Draw(CDC* pDC) const;
	void Draw(CDC* pDC, CRect& rect, int origWidth = 0, int origHeight = 0) const;

private:
	int m_width;
	int m_height;

	unsigned char* data;

	inline int GetStrideLength() const {
		return 4 * ((m_width * 3 + 3) / 4);
	}
};

