#include "pch.h"
#include "Label.h"


Label::Label(const wchar_t* text)
{
	this->text = text;
}

int Label::draw(int verticalOffset)
{
	D2D1::ColorF adjustedColor = Menu::primary;
	if (selected) {
		adjustedColor.r *= 0.75;
		adjustedColor.g *= 0.75;
		adjustedColor.b *= 0.75;
	}

	renderer->DrawFilledRect(0, verticalOffset, animatedWidth, height + verticalOffset, adjustedColor);

	if (animatedWidth != 0)
		renderer->DrawText(0, verticalOffset, animatedWidth, height + verticalOffset, text, Menu::tertiary);

	return height;
}

void Label::toggle()
{
	IComponent::toggle();

	if (callback != nullptr)
		callback(this, 3);
}