#include "pch.h"
#include "Window.h"

Window::Window(HINSTANCE instance, WNDPROC wndProc, void* tfApp)
{
	this->instance = instance;

	gameWindow = FindWindow(NULL, L"Titanfall 2");

	// Create class
	ZeroMemory(&winClass, sizeof(winClass));
	winClass.cbSize = sizeof(winClass);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = wndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = sizeof(LONG_PTR);
	winClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	winClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	winClass.lpszClassName = L"WinClass";
	winClass.hInstance = instance;

	HRESULT hr = RegisterClassEx(&winClass);
	if (!SUCCEEDED(hr)) {
		std::cout << "Failed to register class" << std::endl;
	}
	else {
		// Create the Overlay.
		overlay = CreateWindowEx(
			WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
			L"WinClass",
			L"Titanfall 2",
			WS_POPUPWINDOW ,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			instance,
			tfApp
		);

		if (!overlay) {
			std::cout << "Failed to make overlay" << std::endl;
		}
		else {
			SetLayeredWindowAttributes(overlay, RGB(0, 0, 0), 150, LWA_COLORKEY | LWA_ALPHA);

			ShowWindow(overlay, SW_SHOWNORMAL);
			PositionWindow();

			SetLayeredWindowAttributes(overlay, 0, 1.0f, LWA_ALPHA);
			SetLayeredWindowAttributes(overlay, 0, RGB(0, 0, 0), LWA_COLORKEY);
			
			d2d = new D2D(overlay, GetWindowPos());
		}
	}
}

Window::~Window()
{
	delete d2d;
	Close();
}

RECT Window::GetWindowPos()
{
	RECT rect;
	GetWindowRect(gameWindow, &rect);
	return rect;
}

RECT Window::GetClientPos()
{
	RECT rect;
	GetClientRect(gameWindow, &rect);
	return rect;
}

HWND Window::GetOverlay()
{
	return overlay;
}

void Window::Close()
{
	CloseWindow(overlay);
	d2d->Release();
	UnregisterClass(L"WinClass", instance);
}

void Window::PositionWindow()
{
	int dimensions[4];
	RECT w = GetWindowPos();
	RECT c = GetClientPos();
	int titleBarHeight = (w.bottom - w.top) - c.bottom;
	int border = ((w.right - w.left) - c.right) / 2;
	
	dimensions[0] = w.left + border;
	dimensions[1] = w.top + titleBarHeight - border;
	dimensions[2] = w.right - w.left - border * 2;
	dimensions[3] = w.bottom - w.top - titleBarHeight;

	RECT r;
	r.left = dimensions[0];
	r.top = dimensions[1];
	r.right = r.left + dimensions[2];
	r.bottom = r.top + dimensions[3];

	//SetWindowPos(overlay, NULL, r.left + border, r.top + titleBarHeight - border, r.right - r.left - border * 2, r.bottom - r.top - titleBarHeight, SWP_SHOWWINDOW);
	SetWindowPos(overlay, NULL, r.left, r.top, r.right-r.left, r.bottom-r.top, SWP_SHOWWINDOW);
}