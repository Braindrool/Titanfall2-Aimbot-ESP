#include "pch.h"
#include "D2D.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

template<class Interface>
inline void SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

D2D::D2D(HWND overlayHwnd, RECT renderRect)
{
	this->overlayHwnd = overlayHwnd;
	this->renderRect = renderRect;

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_WARNING;

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &factory);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(writeFactory),
		reinterpret_cast<IUnknown**>(&writeFactory)
	);

	writeFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"", //locale
		&format
	);

	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// Create a Direct2D render target
	renderTarget = NULL;
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			overlayHwnd,
			D2D1::SizeU(
				renderRect.right - renderRect.left,
				renderRect.bottom - renderRect.top)
		),
		&renderTarget
	);

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);
}

D2D::~D2D()
{
	Release();
}

void D2D::Release()
{
	SafeRelease(&factory);
	SafeRelease(&renderTarget);
	SafeRelease(&brush);
	SafeRelease(&writeFactory);
	SafeRelease(&format);
}

void D2D::BeginDrawing()
{
	renderTarget->BeginDraw();
}

void D2D::EndDrawing()
{
	renderTarget->EndDraw();
}

void D2D::DrawLine(int x1, int y1, int x2, int y2)
{
	D2D1_POINT_2F p1, p2;
	p1.x = x1;
	p1.y = y1;
	p2.x = x2;
	p2.y = y2;
	renderTarget->DrawLine(p1, p2, brush);
}

/*void D2D::DrawBox(int x1, int y1, int x2, int y2)
{
	D2D1_RECT_F rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	renderTarget->DrawRectangle(rect, brush);
}*/

void D2D::DrawFilledRect(int x1, int y1, int x2, int y2, D2D1::ColorF color)
{
	D2D1_RECT_F rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	SetColor(color);
	renderTarget->FillRectangle(rect, brush);
}

void D2D::DrawCircle(int x, int y, int radius)
{
	D2D1_ELLIPSE el;
	D2D1_POINT_2F p;
	p.x = x;
	p.y = y;
	el.point = p;
	el.radiusX = radius;
	el.radiusY = radius;

	renderTarget->DrawEllipse(el, brush);
}

/*void D2D::DrawTextBox(int x, int y, std::string text)
{
	std::wstring widestr = std::wstring(text.begin(), text.end());
	const wchar_t* widecstr = widestr.c_str();
	D2D1_RECT_F r;
	r.left = x - 50;
	r.right = x + 50;
	r.top = y - 50;
	r.bottom = y + 50;
	renderTarget->DrawTextW(widecstr, text.length(), format, r, brush);
}*/

void D2D::DrawText(int x1, int y1, int x2, int y2, std::wstring text, D2D1::ColorF color)
{
	D2D1_RECT_F rc = { x1, y1, x2, y2 };
	SetColor(color);
	renderTarget->DrawTextW(text.c_str(), text.length(), format, rc, brush);
}

void D2D::SetColor(D2D1::ColorF color)
{
	brush->SetColor(color);
}

void D2D::Clear(D2D1_COLOR_F color)
{
	renderTarget->Clear(color);
}
