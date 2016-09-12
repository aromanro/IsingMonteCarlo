#pragma once

namespace MonteCarlo {
	class SpinMatrix;
}

class MemoryBitmap
{
public:
	MemoryBitmap();
	~MemoryBitmap();
protected:
	int m_width;
	int m_height;

	unsigned char* data;

	inline int GetStrideLength() {
		return 4 * ((m_width * 3 + 3) / 4);
	}
public:
	void SetSize(int width, int height);

	inline void FillSquare(int Xpos, int Ypos, int size, COLORREF color)
	{
		if (!data) return;

		int stride = GetStrideLength();

		int limX = 3 * min(Xpos + size, m_width);
		int limY = min(Ypos + size, m_height);

		for (int line = Ypos; line < limY; ++line)
		{
			int startLine = (m_height - line - 1) * stride;

			for (int horz = 3 * Xpos; horz < limX; horz += 3)
			{
				int pos = startLine + horz;

				data[pos] = GetBValue(color);
				data[pos + 1] = GetGValue(color);
				data[pos + 2] = GetRValue(color);
			}
		}
	}


	void SetMatrix(MonteCarlo::SpinMatrix& matrix, unsigned int spinsWidth, unsigned int spinsHeight, unsigned int spinSize);

	void Draw(CDC* pDC);
	void Draw(CDC* pDC, CRect& rect, int origWidth = 0, int origHeight = 0);
};

