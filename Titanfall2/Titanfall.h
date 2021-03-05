#pragma once
#include "EntityList.h"
#include "Window.h"
#include "WorldToScreen.h"
#include "Player.h"
#include "Skeleton.h"
#include "Ray.h"
#include "RayHits.h"
#include "Menu.h"
#include "Switch.h"
#include "Label.h"
#include "Slider.h"
#include "ScreenshotCleaner.h"

#define KRABER_SPEED 0.01

class Titanfall
{
public:
	Titanfall(HINSTANCE instance);

	bool Run();
	void OnRender();
	void Cleanup();
	void AimTick();
	void Rerender();
	Player* GetPlayer();

	Window* win;
	MSG msg;

	static int fov;
	static float aimSpeed;
	static bool espOn, aimbotOn, espShowingAi, espShowingFriendly, aimbotTargetingAI;
private:
	Player player;
	EntityList entityList;

	Menu* menu;
	Dropdown* espDropdown, *aimbotDropdown, *aboutDropdown;
	Switch* espActive, * aimbotActive, * espShowAi, * espShowFriendly, * aimbotTargetAi;
	Label* about;
	Slider* fovSlider, * speedSlider;
	
	ScreenshotCleaner cleaner;
};

