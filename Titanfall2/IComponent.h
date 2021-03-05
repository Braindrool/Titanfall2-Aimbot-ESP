#pragma once
#include "IRenderer.h"
#include <vector>

typedef void(__stdcall* ComponentCallback)(int data);

class IComponent
{
public:
	IComponent();
	~IComponent();

	virtual int draw(int verticalOffset) = 0;
	// Returns a value 0-1 representing how far the animation has progressed
	float getAnimationProgress();

	// Flips the component's state and begins its animation
	virtual void toggle();
	virtual void altToggle();
	bool getToggle();
	static IRenderer* renderer;
	std::vector<IComponent*> children;
	static int width;
	static int animatedWidth;
	bool selected = false;
	bool collapsable;
	void(__stdcall* callback)(IComponent*, int);

protected:
	int height = 50;

private:
	float animationSpeed = 4;

	// Animation timestamps
	uint64_t animationStart, animationEnd;

	// Is the component open/activated?
	bool toggled = false;
};
