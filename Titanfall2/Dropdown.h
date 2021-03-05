#pragma once
#include "IComponent.h"
#include "Menu.h"

class Dropdown :
	public IComponent
{
public:
	Dropdown(const wchar_t* text, const wchar_t* icon = L"");

	int draw(int verticalOffset);

private:
	const wchar_t* text;
	const wchar_t* icon;
};
