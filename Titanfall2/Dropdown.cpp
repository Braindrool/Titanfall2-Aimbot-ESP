#include "pch.h"
#include "Dropdown.h"

Dropdown::Dropdown(const wchar_t* text, const wchar_t* icon)
{
	this->text = text;
	this->icon = icon;
	this->collapsable = true;
}

int Dropdown::draw(int verticalOffset)
{
	if ((GetAsyncKeyState(VK_HOME) & 1)) {
		toggle();
	}

	// Draw and animate the children
	int spaceUsed = height;
	for (unsigned int i = 0; i < children.size(); i++) {
		if (getToggle())
			spaceUsed += children[i]->draw(verticalOffset + spaceUsed * getAnimationProgress());
		else
			spaceUsed += children[i]->draw(verticalOffset + spaceUsed - spaceUsed * getAnimationProgress());
	}

	D2D1::ColorF adjustedColor = Menu::primary;
	if (selected) {
		adjustedColor.r *= 0.75;
		adjustedColor.g *= 0.75;
		adjustedColor.b *= 0.75;
	}

	// Draw the backdrop and text
	renderer->DrawFilledRect(0, verticalOffset, animatedWidth, height + verticalOffset, adjustedColor);

	if (animatedWidth != 0) {
		renderer->DrawText(0, verticalOffset, animatedWidth / 4, height + verticalOffset, icon, Menu::tertiary);
		renderer->DrawText(0, verticalOffset, animatedWidth, height + verticalOffset, text, Menu::tertiary);
		renderer->DrawText(animatedWidth - animatedWidth / 4, verticalOffset, animatedWidth, height + verticalOffset, getToggle() ? L"˅" : L">", Menu::tertiary);
	}

	if (getToggle())
		spaceUsed = height * (1 - getAnimationProgress()) + spaceUsed * getAnimationProgress();
	else
		spaceUsed = spaceUsed - (spaceUsed - height) * getAnimationProgress();
	return spaceUsed;
}