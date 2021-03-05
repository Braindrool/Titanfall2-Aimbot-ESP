#pragma once
#include "IComponent.h"
#include "Menu.h"

class Switch :
	public IComponent
{
public:
	Switch(const wchar_t* text);

	int draw(int verticalOffset);
	void toggle();

private:
	const wchar_t* text;
};
