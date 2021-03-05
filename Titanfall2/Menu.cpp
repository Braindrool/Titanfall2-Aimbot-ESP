#include "pch.h"
#include "Menu.h"
#include "IComponent.h"

D2D1::ColorF Menu::primary = D2D1::ColorF(D2D1::ColorF::DarkSlateGray);// { 50, 250, 0, 255 };
D2D1::ColorF Menu::secondary = D2D1::ColorF(D2D1::ColorF::Red);// { 0, 225, 0, 255 };// { 26, 25, 24, 0 };
D2D1::ColorF Menu::tertiary = D2D1::ColorF(D2D1::ColorF::Orange);// { 0, 88, 0, 255 };

Menu::Menu(IRenderer* renderer, int width, int screenHeight)
{
	IComponent::renderer = renderer;
	IComponent::width = width;

	this->screenHeight = screenHeight;
}

int Menu::draw(int verticalOffset)
{
	if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		toggle();

	if (getToggle())
		animatedWidth = width * getAnimationProgress();
	else
		animatedWidth = width - width * getAnimationProgress();

	// Back panel
	// renderer->DrawFilledRect(0, 0, animatedWidth, screenHeight, Menu::secondary);

	int offset = 0, previousOffset = offset;
	for (unsigned int i = 0; i < children.size(); i++) {
		previousOffset = offset;

		offset += children[i]->draw(offset);

		// Dividers
		renderer->DrawFilledRect(0, previousOffset - 1, animatedWidth, previousOffset, Menu::tertiary);
	}

	//renderer->DrawFilledRect(animatedWidth - 4, 0, animatedWidth, screenHeight, Menu::tertiary);

	return offset;
}

void Menu::checkInputs()
{
	if (GetAsyncKeyState(VK_NUMPAD8) & 1)
		selectPrevious();

	if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		selectNext();

	if (GetAsyncKeyState(VK_NUMPAD6) & 1)
		interactSelected();

	if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		altInteract();
}

void Menu::interactSelected()
{
	if (selectedComponent == nullptr)
		return;

	std::vector<IComponent*> menuOrder = getMenuOrder();

	// Close everything except the selected component. Ignore non-collapsables
	for (unsigned int i = 0; i < menuOrder.size(); i++) {
		if (menuOrder[i]->getToggle()
			&& menuOrder[i] != selectedComponent
			&& menuOrder[i]->collapsable
			&& selectedComponent->collapsable) {
			menuOrder[i]->toggle();
		}
		else if (menuOrder[i] == selectedComponent) {
			selectedComponent->toggle();
		}
	}

	// The menu order changes when a dropdown is expanded. Find the new index
	menuOrder = getMenuOrder();
	for (unsigned int i = 0; i < menuOrder.size(); i++) {
		if (selectedComponent == menuOrder[i]) {
			selectedComponent->selected = true; // Needed
			selectedIndex = i;
			break;
		}
	}
}

void Menu::altInteract()
{
	if (selectedComponent != nullptr)
		selectedComponent->altToggle();
}

IComponent* Menu::offsetSelection(int change)
{
	std::vector<IComponent*> menuOrder = getMenuOrder();

	selectedIndex += change;

	if (selectedIndex >= (int)menuOrder.size())
		selectedIndex = 0;
	if (selectedIndex < 0)
		selectedIndex = menuOrder.size() - 1;

	selectedComponent = menuOrder[selectedIndex];
	selectedComponent->selected = true;
	return selectedComponent;
}

IComponent* Menu::selectNext()
{
	return offsetSelection(1);
}

IComponent* Menu::selectPrevious()
{
	return offsetSelection(-1);
}

std::vector<IComponent*> Menu::getMenuOrder()
{
	std::vector<IComponent*> menuOrder;

	// TODO: Change to recursive function to allow nested dropdowns
	// Get the order of the menu items
	for (unsigned int i = 0; i < children.size(); i++) {
		menuOrder.push_back(children[i]);
		children[i]->selected = false;
		if (children[i]->getToggle()) {
			for (unsigned int j = 0; j < children[i]->children.size(); j++) {
				children[i]->children[j]->selected = false;
				menuOrder.push_back(children[i]->children[j]);
			}
		}
	}

	return menuOrder;
}