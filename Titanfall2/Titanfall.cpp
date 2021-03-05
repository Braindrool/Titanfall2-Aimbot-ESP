#include "pch.h"
#include "Titanfall.h"

bool Titanfall::espOn = true;
bool Titanfall::aimbotOn = true;
bool Titanfall::espShowingAi = false;
bool Titanfall::espShowingFriendly = false;
bool Titanfall::aimbotTargetingAI = false;
int Titanfall::fov = 350;
float Titanfall::aimSpeed = 0.25;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void __stdcall espActiveCallback(IComponent* component, int data);
void __stdcall aimActiveCallback(IComponent* component, int data);
void __stdcall espTeamCallback(IComponent* component, int data);
void __stdcall espShowAiCallback(IComponent* component, int data);
void __stdcall aimAICallback(IComponent* component, int data);
void __stdcall aimFovCallback(IComponent* component, int data);
void __stdcall speedCallback(IComponent* component, int data);

Titanfall::Titanfall(HINSTANCE instance)
{
	// Create the overlay
	win = new Window(instance, WndProc, this);

	// Set up the player
	player.entity = new Entity();
	player.win = win;

	// Give the Skeleton drawer a ptr to the window
	Skeleton::SetWindow(win);

	Addresses::Init();

	// UI
	menu = new Menu(win->d2d, 300, win->GetWindowPos().bottom - win->GetWindowPos().top);
	espDropdown = new Dropdown(L"ESP", L"👁");
	aimbotDropdown = new Dropdown(L"Aimbot", L"🔫");
	aboutDropdown = new Dropdown(L"About", L"?");
	espActive = new Switch(L"Active");
	aimbotActive = new Switch(L"Active");
	espShowAi = new Switch(L"Show AI");
	espShowFriendly = new Switch(L"Friendlies");
	aimbotTargetAi = new Switch(L"Target AI");
	about = new Label(L"Made by Braindrool");
	fovSlider = new Slider(L"FOV", 350, 0, 600);
	speedSlider = new Slider(L"Speed", 75, 0, 300);

	menu->children.push_back(aimbotDropdown);
	menu->children.push_back(espDropdown);
	menu->children.push_back(aboutDropdown);

	espDropdown->children.push_back(espActive);
	espDropdown->children.push_back(espShowAi);
	espDropdown->children.push_back(espShowFriendly);
	aimbotDropdown->children.push_back(aimbotActive);
	aimbotDropdown->children.push_back(aimbotTargetAi);
	aimbotDropdown->children.push_back(fovSlider);
	aimbotDropdown->children.push_back(speedSlider);
	aboutDropdown->children.push_back(about);

	espActive->callback = espActiveCallback;
	aimbotActive->callback = aimActiveCallback;
	espShowAi->callback = espShowAiCallback;
	espShowFriendly->callback = espTeamCallback;
	aimbotTargetAi->callback = aimAICallback;
	fovSlider->callback = aimFovCallback;
	speedSlider->callback = speedCallback;

	espActive->toggle();
	aimbotActive->toggle();
}

// Main Loop
bool Titanfall::Run()
{
	std::clock_t start = std::clock();
	menu->checkInputs();
	entityList.Refresh();
	//std::cout << "Entity list time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	// Reset the player vars
	player.closestDistToCrosshair = -1;
	player.closestToCrosshair = nullptr;

	// Set the player data
	player.entity->data = (EntityData*) Addresses::Get("localplayer");

	//Test cleaning the screenshots
	/*if (GetAsyncKeyState(VK_NUMPAD9) & 1)
	{
		cleaner.SaveTestScreenshot(FindWindow(NULL, L"Titanfall 2"));
	}*/

	// Loop through the entities
	for (auto const& entry : entityList.Entities) {
		Entity* entity = entry.second;

		if (entity->IsSafeToAccess()) {
			// Find the local player
			if (entity->IsLocalPlayer()) {
				//std::cout << entity->data << std::endl;
				//player.entity->data = entity->data;
				continue;
			}

			if (player.entity != nullptr && player.entity->IsSafeToAccess()) {
				// Find the closest enemy to the crosshairs
				if ((entity->IsPlayer() || (entity->IsAI() && aimbotTargetingAI)) && entity->IsAlive() && !entity->IsLocalPlayer() && !entity->IsTitan() && !entity->IsFriendly()) {
					float distToCrosshair = player.DistanceFromCrosshair(entity);

					// New closest entity found
					if (distToCrosshair < fov && (distToCrosshair < player.closestDistToCrosshair || player.closestDistToCrosshair == -1)) {
						player.closestDistToCrosshair = distToCrosshair;
						player.closestToCrosshair = entity;
					}
				}
			}
		}
	}

	// Lock onto the entity the player was aiming at if one exists and holding button
	if (player.entity != nullptr && player.entity->IsSafeToAccess()) {
		if (player.closestToCrosshair != nullptr
			&& player.closestToCrosshair->IsSafeToAccess()
			&& player.IsAiming()
			&& player.lockedEntity == nullptr) {

			player.lockedEntity = player.closestToCrosshair;
			//std::cout << std::hex << *(int*)((BYTE*)player.lockedEntity->data + 0x38c ) << " : " << player.lockedEntity->data << std::endl;
		}
		// Release the lock
		else if (!player.IsAiming()) {
			player.AimRelease();
		}
	}


	// Check to see if any messages are waiting in the queue
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Translate the message and dispatch it to WindowProc()
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check for quit
	if (msg.message == WM_QUIT || GetAsyncKeyState(VK_END))
		return false;;

	return true;
}

// Render Loop
void Titanfall::OnRender()
{
	//std::clock_t start = std::clock();
	entityList.Refresh();

	// Clear the screen
	win->d2d->Clear(D2D1::ColorF(0, 0, 0, 0));

	if(aimbotOn)
		win->d2d->DrawCircle((win->GetWindowPos().right - win->GetWindowPos().left) / 2, (win->GetWindowPos().bottom - win->GetWindowPos().top) / 2, fov);

	for (auto const& entry : entityList.Entities) {
		Entity* entity = entry.second;

		if (entity->IsSafeToAccess() && espOn) {
			if (entity->IsLocalPlayer())
				continue;

			if ((entity->IsPlayer() || (entity->IsAI() && espShowingAi)) && entity->IsAlive() && !entity->IsTitan() && (!entity->IsFriendly() || espShowingFriendly)) {
				if(entity->IsFriendly())
					win->d2d->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
				else
					win->d2d->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
				
				Skeleton::DrawSkeleton(entity);
			}
		}
	}

	menu->draw(0);

	//std::cout << "Draw time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}


void Titanfall::Cleanup()
{
	cleaner.unhook();
	win->Close();
	entityList.Cleanup();
}

// Smooth aim
void Titanfall::AimTick()
{
	// Aim at the locked entity
	if (player.lockedEntity != nullptr && aimbotOn) {
		player.aimSpeed = Titanfall::aimSpeed;

		if (player.lockedEntity->IsSafeToAccess() && player.lockedEntity->IsAlive() && player.IsAiming()) {
			player.AimAt(player.lockedEntity, "chest");
		}
		else {
			player.AimRelease();
		}
	}
}

void Titanfall::Rerender()
{
	const RECT r = win->GetClientPos();
	InvalidateRect(win->GetOverlay(), &r, false);
	//UpdateWindow(win->GetOverlay());
}

Player* Titanfall::GetPlayer()
{
	return &player;
}

void __stdcall espActiveCallback(IComponent* component, int data)
{
	Titanfall::espOn = (bool)data;
}

void __stdcall espTeamCallback(IComponent* component, int data)
{
	Titanfall::espShowingFriendly = (bool)data;
}

void __stdcall espShowAiCallback(IComponent* component, int data)
{
	Titanfall::espShowingAi = (bool)data;
}

void __stdcall aimActiveCallback(IComponent* component, int data)
{
	Titanfall::aimbotOn = (bool)data;
}

void __stdcall aimAICallback(IComponent* component, int data)
{
	Titanfall::aimbotTargetingAI = (bool)data;
}

void __stdcall aimFovCallback(IComponent* component, int data)
{
	Titanfall::fov = data;
}

void __stdcall speedCallback(IComponent* component, int data)
{
	Titanfall::aimSpeed = (float)data / 300;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Titanfall* tfApp = (Titanfall*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(tfApp)
		);

		return 1;
	}
	break;
	case WM_PAINT:
	{
		Titanfall* tfApp = reinterpret_cast<Titanfall*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hWnd,
				GWLP_USERDATA
			)));
		if (tfApp->win->d2d != NULL) {
			tfApp->win->d2d->BeginDrawing();
			tfApp->OnRender();
			tfApp->win->d2d->EndDrawing();
		}

		ValidateRect(hWnd, NULL);

		return 0;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}