#include "pch.h"
#include "Switch.h"


Switch::Switch(const wchar_t* text)
{
	this->text = text;
}

int Switch::draw(int verticalOffset)
{
	D2D1::ColorF adjustedColor = Menu::primary;
	if (selected) {
		adjustedColor.r *= 0.75;
		adjustedColor.g *= 0.75;
		adjustedColor.b *= 0.75;
	}

	// Draw the bordering box
	renderer->DrawFilledRect(0, verticalOffset, animatedWidth, height + verticalOffset, adjustedColor);
	renderer->DrawFilledRect(animatedWidth - animatedWidth / 3, verticalOffset + height * 0.25, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.25 + 2, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth - animatedWidth / 3, verticalOffset + height * 0.75 - 2, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.75, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth - animatedWidth / 3 - 2, verticalOffset + height * 0.25, animatedWidth - animatedWidth / 3, verticalOffset + height * 0.75, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth - animatedWidth / 10 - 2, verticalOffset + height * 0.25, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.75, Menu::tertiary);

	float mult;
	if (getToggle())
		mult = getAnimationProgress();
	else
		mult = 1 - getAnimationProgress();

	renderer->DrawFilledRect(animatedWidth - animatedWidth / 3 + (animatedWidth / 3 - animatedWidth / 4.65) * mult, verticalOffset + height * 0.25, animatedWidth - animatedWidth / 4.65 + (animatedWidth / 3 - animatedWidth / 4.65) * mult, verticalOffset + height * 0.75, Menu::tertiary);

	if (animatedWidth != 0)
		renderer->DrawText(0, verticalOffset, animatedWidth / 2, height + verticalOffset, text, Menu::tertiary);

	return height;
}

void Switch::toggle()
{
	IComponent::toggle();

	if (callback != nullptr)
		callback(this, (int)getToggle());
}
