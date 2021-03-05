#include "pch.h"
#include "ScreenshotCleaner.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <windows.h>
#include <ShlObj.h>

bool ScreenshotCleaner::Safe;
using tBitBlt = BOOL(WINAPI*)(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);

tBitBlt oBitBlt;

class BMP_T
{
public:
	BITMAPFILEHEADER   bmfHeader{ 0 };
	BITMAPINFOHEADER   bi{ 0 };
	BYTE* bmp{ nullptr };
	DWORD dwBmpSize{ 0 };

	BMP_T();

	BMP_T(int width, int height);

	~BMP_T();
};

struct MonitorCallbackStruct
{
	MONITORINFOEX* targetMonitor;
};

//Required to grab Desktop HDC for correct monitor
BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprcMonitor, _In_  LPARAM dwData);

ScreenshotCleaner::ScreenshotCleaner()
{
	hook();
}

void ScreenshotCleaner::hook()
{
	void* src = (BYTE*)GetProcAddress(GetModuleHandleA("Gdi32.dll"), "BitBlt");

	BYTE bytes[15] = { 0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xE0, 0x90, 0x90, 0x90 };

	void* trampoline = VirtualAlloc(0, sizeof(bytes), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(stolenBytes, src, 15);

	// Write the absolute address to the jmp bytes
	uintptr_t absoluteAddress = (uintptr_t)hkBitBlt;
	for (int i = 0; i < 8; i++)
		bytes[2 + i] = (absoluteAddress >> (i * 8));

	DWORD oldProtect;
	VirtualProtect(src, 15, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(src, bytes, sizeof(bytes));

	VirtualProtect(src, 15, oldProtect, &oldProtect);

	memcpy(trampoline, stolenBytes, sizeof(stolenBytes));
	absoluteAddress = (uintptr_t)src + 15;

	for (int i = 0; i < 8; i++)
		bytes[2 + i] = (absoluteAddress >> (i * 8));

	memcpy((BYTE*)trampoline+15, bytes, 15);
	oBitBlt = (tBitBlt)trampoline;
}

void ScreenshotCleaner::unhook()
{
	void* src = (BYTE*)GetProcAddress(GetModuleHandleA("Gdi32.dll"), "BitBlt");
	std::cout << std::hex << "unhook: " << src << std::endl;
	DWORD oldProtect;
	VirtualProtect(src, 15, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(src, stolenBytes, 15);
	VirtualProtect(src, 15, oldProtect, &oldProtect);
}

BOOL WINAPI hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	HWND targetWindow = FindWindow(L"WinClass", L"Titanfall 2");

	BOOL s1 = ShowWindow(targetWindow, SW_HIDE);
	BOOL result = oBitBlt(hdcDst, x, y, cx, cy, hdcSrc, x1, y1, rop);
	BOOL s2 = ShowWindow(targetWindow, SW_SHOW);
	std::cout << "screenshot cleaned" << std::endl;
	return  result;
}

std::wstring ScreenshotCleaner::GetTimeString()
{
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	localtime_s(&now_tm, &now_c);
	std::wstringstream wss;

	wss << std::put_time(&now_tm, L"%mm-%dd-%Hh-%Mm-%Ss");
	wss << '-' << std::setfill(L'0') << std::setw(3) << ms.count() << L"ms";

	return wss.str();
}

std::wstring ScreenshotCleaner::GetFileName()
{
	wchar_t* path = nullptr;
	SHGetKnownFolderPath(FOLDERID_Desktop, 0, 0, &path);
	if (GetLastError() == 1008) SetLastError(0); //get rid of meaningless error

	std::wstring wpath(std::wstring(path) + L"\\screenshots\\");

	CoTaskMemFree(path);
	CreateDirectory(wpath.c_str(), NULL);
	auto timetext = GetTimeString();

	return wpath + timetext + L".bmp";
}

void ScreenshotCleaner::SaveBitBltResult(HDC hdcDst, HDC hdcSrc, int width, int height, std::wstring fileSuffix)
{
	// Setup bitmap structure
	auto hbitmapsrc = CreateCompatibleBitmap(hdcDst, width, height);
	HBITMAP hbitmapdst = (HBITMAP)SelectObject(hdcDst, hbitmapsrc);
	BMP_T bmp_t(width, height);

	//GetDIBits() = Convert screenshot data to bitmap structure
	GetDIBits(hdcSrc, hbitmapdst, 0, (UINT)height, bmp_t.bmp, (BITMAPINFO*)&bmp_t.bi, DIB_RGB_COLORS);

	//Begin Saving File to Disk
	auto path = GetFileName();
	path.insert(path.find(L".bmp"), fileSuffix);

	auto hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (void*)&bmp_t.bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)&bmp_t.bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)bmp_t.bmp, bmp_t.dwBmpSize, &dwBytesWritten, NULL);

	CloseHandle(hFile);
}

BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprcMonitor, _In_  LPARAM dwData)
{
	// for each monitor, compare the device name of the monitor with the
	// monitor name of the monitor which hosts our game window

	MonitorCallbackStruct& data = *(MonitorCallbackStruct*)dwData;

	MONITORINFOEX monInfo;
	monInfo.cbSize = sizeof(monInfo);
	GetMonitorInfo(hMonitor, &monInfo);

	if (!wcscmp(monInfo.szDevice, data.targetMonitor->szDevice))
	{
		auto hdcSrc = DevC;

		auto width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
		auto height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;

		//device found

		auto hdcDst = CreateCompatibleDC(hdcSrc);
		auto hbitmap = CreateCompatibleBitmap(hdcSrc, width, height);

		SelectObject(hdcDst, hbitmap);

		//We are saving a new test screenshot, to test our hook
		//This call, will trigger our hook, our hook will save 1 cleaned screenshot and 1 dirty screenshot
		auto bResult = BitBlt(hdcDst, 0, 0, width, height, hdcSrc, monInfo.rcMonitor.left, monInfo.rcMonitor.top, SRCCOPY);//RCCOPY | CAPTUREBBLT

		//At this point our cleaned screenshot was saved in the hook
		/*if (!sshotCleaner.bEnabled)
		{
			sshotCleaner.SaveBitBltResult(hdcDst, hdcSrc, width, height, L"_regular_test");
			LOG(L"SShotCleaner was Disabled, test screenshot saved as is");
		}*/
		ScreenshotCleaner::SaveBitBltResult(hdcDst, hdcSrc, width, height, L"_regular_test");
	}

	return TRUE;
}

void ScreenshotCleaner::SaveTestScreenshot(HWND hWnd)
{
	// === Get DC for correct monitor for game window, ensures overlays are captured ===
	auto hMon = MonitorFromWindow(FindWindow(NULL, L"Titanfall 2"), MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX monInfo;
	monInfo.cbSize = sizeof(monInfo);
	GetMonitorInfo(hMon, &monInfo);

	MonitorCallbackStruct monCallBackStruct;
	monCallBackStruct.targetMonitor = &monInfo;
	EnumDisplayMonitors(GetDC(NULL), NULL, MonitorEnumProcCallback, (LPARAM)&monCallBackStruct);
}

BMP_T::BMP_T() {}

BMP_T::BMP_T(int width, int height)
{
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;

	// Add the size of the headers to the size of the bitmap to get the total file size
	bmfHeader.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //"BM"

	if (bmp)
		delete[] bmp;//

	bmp = new BYTE[dwBmpSize];
}

BMP_T::~BMP_T()
{
	delete[] bmp;
}
