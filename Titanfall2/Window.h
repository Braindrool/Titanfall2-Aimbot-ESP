#pragma once
#include "D2D.h"

class Window
{
public:
	Window(HINSTANCE instance, WNDPROC wndProc, void* l4d2App);
	~Window();

	RECT GetWindowPos();
	RECT GetClientPos();
	HWND GetOverlay();
	void Close();

	// Position overlay over game window
	void PositionWindow();

	D2D* d2d;

private:
	HINSTANCE instance;
	HWND overlay, gameWindow;
	WNDCLASSEX winClass;
};