#pragma once
#include "IRenderer.h"
#include "IComponent.h"
#include "Dropdown.h"
#include <typeinfo>
class Menu : public IComponent
{
public:
	Menu(IRenderer* renderer, int width, int screenHeight);

	int draw(int verticalOffset);
	void checkInputs();
	void interactSelected();
	void altInteract();
	IComponent* offsetSelection(int change);
	IComponent* selectNext();
	IComponent* selectPrevious();

	int selectedIndex = 0;
	IComponent* selectedComponent;

	//static IRenderer::Color secondary;
	//static IRenderer::Color primary;
	//static IRenderer::Color tertiary;
	static D2D1::ColorF primary, secondary, tertiary;
private:
	std::vector<IComponent*> getMenuOrder();
	int screenHeight;
};