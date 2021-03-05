#pragma once
// https://guidedhacking.com/threads/screenshot-cleaner-bypass-fairfight-punkbuster-bitblt-hook.15216/
class ScreenshotCleaner
{
public:
	ScreenshotCleaner();
	void unhook();

	static std::wstring GetFileName();
	static std::wstring GetTimeString();

	//After BitBlt() is called, use GetDIBits to save the screenshot
	static void SaveBitBltResult(HDC hdcDst, HDC hdcSrc, int width, int height, std::wstring fileSuffix);

	//Used for testing the screenshot cleaner before deployment
	static void SaveTestScreenshot(HWND hWnd);

	static bool Safe;
private:
	void hook();
	BYTE stolenBytes[15];
};

BOOL WINAPI hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
