#pragma once
#include "../framework.h"
#include <wingdi.h>
#include <vector>

#pragma pack(push)
#pragma pack(4)
struct TtRGBA
{
	BYTE Blue;
	BYTE Green;
	BYTE Red;
	BYTE Alpha;

	TtRGBA()
	{
		Red = 0;
		Green = 0;
		Blue = 0;
		Alpha = 0;
	}
	TtRGBA(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		Red = r;
		Green = g;
		Blue = b;
		Alpha = a;
	}
};
#pragma pack(pop)

class TtBitmap
{
	HBITMAP		mBitmap = nullptr;
	HDC			mMemHDC = nullptr;
	BITMAPINFOHEADER	mBmpHead{};
	std::vector<TtRGBA>	mPixels;
public:
	TtBitmap();
	~TtBitmap();
	void Cleanup();
	int GetWidth() {
		return mBmpHead.biWidth;
	}
	int GetHeight() {
		return mBmpHead.biHeight;
	}
	void SetPixel(int x, int y, TtRGBA color)
	{
		mPixels[mBmpHead.biWidth * y + x] = color;
	}
	TtRGBA& GetPixel(int x, int y)
	{
		return mPixels[mBmpHead.biWidth * y + x];
	}
	const TtRGBA& GetPixel(int x, int y) const
	{
		return mPixels[mBmpHead.biWidth * y + x];
	}
	TtRGBA& operator()(int x, int y) {
		return GetPixel(x, y);
	}
	const TtRGBA& operator()(int x, int y) const{
		return GetPixel(x, y);
	}
	bool SetSize(HDC hDC, int w, int h);
	void FlushPixels();
	bool DrawToDC(HDC hDC, int x, int y, int w, int h, int sx, int sy);
};

