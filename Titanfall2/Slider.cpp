#include "pch.h"
#include "Slider.h"

Slider::Slider(const wchar_t* text, float value, float min, float max)
{
	this->value = value;
	this->min = min;
	this->max = max;
	this->text = text;

	height = 100;
}

int Slider::draw(int verticalOffset)
{
	D2D1::ColorF adjustedColor = Menu::primary;
	if (selected) {
		adjustedColor.r *= 0.75;
		adjustedColor.g *= 0.75;
		adjustedColor.b *= 0.75;
	}

	renderer->DrawFilledRect(0, verticalOffset, animatedWidth, height + verticalOffset, adjustedColor);

	// Draw the bordering box
	renderer->DrawFilledRect(animatedWidth / 10, verticalOffset + height * 0.5, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.5 + 2, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth / 10, verticalOffset + height * 0.8 - 2, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.8, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth - animatedWidth / 10 - 2, verticalOffset + height * 0.5, animatedWidth - animatedWidth / 10, verticalOffset + height * 0.8, Menu::tertiary);
	renderer->DrawFilledRect(animatedWidth / 10 - 2, verticalOffset + height * 0.5, animatedWidth / 10, verticalOffset + height * 0.8, Menu::tertiary);

	// Draw the slidy thing. +8 to bar to compensate for slider box size
	float bar = (animatedWidth - (animatedWidth / 10 + 8)) - (animatedWidth / 10);
	float slideBox = bar * value / max;
	renderer->DrawFilledRect(animatedWidth / 11 + slideBox, verticalOffset + height * 0.5, animatedWidth / 8 + slideBox, verticalOffset + height * 0.8, Menu::tertiary);

	if(animatedWidth != 0)
		renderer->DrawText(0, verticalOffset, animatedWidth, height / 2 + verticalOffset, text, Menu::tertiary);

	return height;
}

void Slider::toggle()
{
	IComponent::toggle();

	value += (max - min) / 50;

	if (value > max)
		value = max;
	else if (value < min)
		value = min;

	if(callback != nullptr)
		callback(this, value);
}

void Slider::altToggle()
{
	IComponent::altToggle();

	value -= (max - min) / 50;

	if (value > max)
		value = max;
	else if (value < min)
		value = min;

	if (callback != nullptr)
		callback(this, value);
}