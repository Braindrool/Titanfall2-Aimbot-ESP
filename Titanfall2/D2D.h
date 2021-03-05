#pragma once
#include "IRenderer.h"

#define PI 3.1415926535

class D2D : public IRenderer
{
public:
	D2D(HWND overlayHwnd, RECT renderRect);
	~D2D();

	void BeginDrawing();
	void EndDrawing();

	void DrawLine(int x1, int y1, int x2, int y2);
	//void DrawBox(int x1, int y1, int x2, int y2);
	void DrawFilledRect(int x1, int y1, int x2, int y2, D2D1::ColorF color);
	void DrawCircle(int x, int y, int radius);
	//void DrawTextBox(int x, int y, std::string text);
	void DrawText(int x1, int y1, int x2, int y2, std::wstring text, D2D1::ColorF color);
	void SetColor(D2D1::ColorF color);
	void Clear(D2D1_COLOR_F color);
	void Release();

private:
	HWND overlayHwnd;
	RECT renderRect;

	ID2D1Factory* factory;
	IDWriteFactory* writeFactory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;
	IDWriteTextFormat* format;
};