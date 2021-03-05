#include "pch.h"
#include "IComponent.h"

IRenderer* IComponent::renderer;
int IComponent::width;
int IComponent::animatedWidth;

IComponent::IComponent()
{
}


IComponent::~IComponent()
{
}


void IComponent::toggle()
{
	toggled = !toggled;

	// Set animation timestamps
	animationStart = GetTickCount64();
	animationEnd = (uint64_t)(animationStart + (1 / animationSpeed) * 1000);
}

void IComponent::altToggle()
{
	// Intentionally left blank
}

bool IComponent::getToggle()
{
	return toggled;
}

float IComponent::getAnimationProgress()
{
	float progress = 1 - (float)((int64_t)animationEnd - (int64_t)GetTickCount64()) / (animationEnd - animationStart);

	if (progress < 0)
		progress = 0;
	else if (progress > 1)
		progress = 1;

	return progress;
}