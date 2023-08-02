#include "TtBitmap.h"
#include "TtApplication.h"

TtBitmap::TtBitmap()
{
}
TtBitmap::~TtBitmap()
{
	Cleanup();
}
void TtBitmap::Cleanup()
{
	if (mMemHDC != nullptr)
	{
		::DeleteDC(mMemHDC);
		mMemHDC = nullptr;
	}
	if (mBitmap != nullptr)
	{
		::DeleteObject(mBitmap);
		mBitmap = nullptr;
	}
}
void TtBitmap::FlushPixels()
{
	::SetBitmapBits(mBitmap, (DWORD)mPixels.size() * sizeof(TtRGBA), &mPixels[0]);
}
bool TtBitmap::SetSize(HDC hDC, int w, int h)
{
	Cleanup();
	if (w == 0 || h == 0)
		return false;
	mBmpHead.biWidth = w;
	mBmpHead.biHeight = h;
	mPixels.resize(sizeof(TtRGBA) * w * h);
	mBitmap = ::CreateBitmap(w, h, 1, 32, &mPixels[0]);
	if (hDC != nullptr)
	{
		mMemHDC = CreateCompatibleDC(hDC);
		SelectObject(mMemHDC, mBitmap);
	}
	return true;
}

bool TtBitmap::LoadBmp(const char* file)
{
	Cleanup();

	std::ifstream bmpfile(file, std::ios::in | std::ios::binary);
	if (bmpfile.is_open() == false)
	{
		return false;
	}
	BITMAPFILEHEADER bmpHeader{};
	bmpfile.read((char*)&bmpHeader, sizeof(bmpHeader));
	if (bmpHeader.bfType != 0x4d42)
		return false;

	bmpfile.read((char*)&mBmpHead, sizeof(mBmpHead));

	mPixels.resize(mBmpHead.biWidth * mBmpHead.biHeight);

	switch (mBmpHead.biBitCount)
	{
		case 24:
			{
				int rowPitch = mBmpHead.biWidth * sizeof(BYTE) * 3;
				for (int y = 0; y < mBmpHead.biHeight; y++)
				{
					for (int x = 0; x < mBmpHead.biWidth; x++)
					{
						bmpfile.read((char*)&mPixels[y * mBmpHead.biWidth + x], sizeof(BYTE) * 3);
					}
				}
			}
			break;
		case 32:
			{
				bmpfile.read((char*)&mPixels[0], sizeof(BYTE) * 4 * mBmpHead.biWidth * mBmpHead.biWidth);
			}
			break;
		default:
			return false;
	}

	bmpfile.close();
	return false;
}

bool TtBitmap::DrawToDC(HDC hDC, int x, int y, int w, int h, int sx, int sy)
{
	return ::BitBlt(hDC, x, y, w, h, mMemHDC, sx, sy, SRCCOPY) ? true : false;
}