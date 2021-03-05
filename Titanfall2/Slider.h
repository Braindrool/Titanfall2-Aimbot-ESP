#pragma once
#include "IComponent.h"
#include "Menu.h"

class Slider :
	public IComponent
{
public:
	Slider(const wchar_t* text, float value, float min, float max);

	int draw(int verticalOffset);
	void toggle();
	void altToggle();

private:
	float value, min, max;
	const wchar_t* text;
};
