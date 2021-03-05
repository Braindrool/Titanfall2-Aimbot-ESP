#include "pch.h"
#include "Player.h"

void Player::AimAt(Vector3 pos)
{
	Vector3 playerPos = *(Vector3*)Addresses::Get("camerapos");// entity->GetBone("head");
	
	float dX = pos.x - playerPos.x;
	float dY = pos.y - playerPos.y;
	float dZ = pos.z - playerPos.z;

	float dist = sqrt(pow(pos.x - playerPos.x, 2) + pow(pos.y - playerPos.y, 2) + pow(pos.z - playerPos.z, 2));

	float aY = -asin(dZ / dist) * (180 / PI);
	float aX = atan2(dY, dX) * (180 / PI);

	// Get the difference between angles and normalize it
	Vector3 deltaAng = { aY - entity->data->ViewAnglesWritable.x, aX - entity->data->ViewAnglesWritable.y };
	float deltaDist = sqrt(pow(deltaAng.x, 2) + pow(deltaAng.y, 2));

	if (deltaDist > aimSpeed) {
		deltaAng.x /= deltaDist;
		deltaAng.y /= deltaDist;

		// Multiply by the aim speed
		deltaAng.x *= aimSpeed;
		deltaAng.y *= aimSpeed;
	}

	// TODO: Turn this into a hook
	// Crashes if not in game, disabled for now
	//uintptr_t weapon = Addresses::Get("primaryweapon");
	//*(float*)(weapon + 0x12F0) = 0; // No spread

	// Smooth aim
	SetViewAngles(entity->data->ViewAnglesWritable.x + deltaAng.x, entity->data->ViewAnglesWritable.y + deltaAng.y);
}

void Player::AimAt(Entity* ent, const char* boneName)
{
	AimAt(ent->GetBone(boneName));
}

void Player::AimAt(Entity* ent)
{
	AimAt(ent->data->Position1);
}

void Player::AimTick()
{
	aimProgress += aimSpeed;
	if (aimProgress > 1)
		aimProgress = 1;
}

void Player::AimRelease()
{
	aimProgress = 0;
	lockedEntity = nullptr;
}


void Player::SetViewAngles(float pitch, float yaw)
{
	entity->data->ViewAnglesWritable.x = pitch;
	entity->data->ViewAnglesWritable.y = yaw;
}

bool Player::IsAiming()
{
	return GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON);
}

float Player::DistanceFromCrosshair(Entity* ent)
{
	Vector2 screen;
	WorldToScreen::D3D(ent->data->Position1, screen, (float*)Addresses::Get("viewmatrix"), win->GetWindowPos().right - win->GetWindowPos().left, win->GetWindowPos().bottom - win->GetWindowPos().top);

	return sqrt(pow((win->GetWindowPos().right - win->GetWindowPos().left) / 2 - screen.x, 2) + pow((win->GetWindowPos().bottom - win->GetWindowPos().top) / 2 - screen.y, 2));
}
