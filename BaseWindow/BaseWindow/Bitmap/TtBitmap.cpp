#include "TtBitmap.h"

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

bool TtBitmap::DrawToDC(HDC hDC, int x, int y, int w, int h, int sx, int sy)
{
	return ::BitBlt(hDC, x, y, w, h, mMemHDC, sx, sy, SRCCOPY) ? true : false;
}