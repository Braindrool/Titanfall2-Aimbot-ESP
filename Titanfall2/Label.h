#pragma once
#include "IComponent.h"
#include "Menu.h"

class Label :
	public IComponent
{
public:
	Label(const wchar_t* text);

	int draw(int verticalOffset);
	void toggle();

private:
	const wchar_t* text;
};
