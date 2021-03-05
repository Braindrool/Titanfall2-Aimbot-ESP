#pragma once
class IRenderer
{
public:
	struct Color { int r, g, b, a; };

	virtual void DrawFilledRect(int x1, int y1, int x2, int y2, D2D1::ColorF color) = 0;
	virtual void DrawText(int x1, int y1, int x2, int y2, std::wstring text, D2D1::ColorF color) = 0;
};