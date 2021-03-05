#pragma once
#include "Entity.h"
#include "Addresses.h"
#include "WorldToScreen.h"
#include "Window.h"

class Player
{
public:
	void AimAt(Vector3 pos);
	void AimAt(Entity* ent, const char* boneName);
	void AimAt(Entity* ent);
	void AimTick();
	void AimRelease();
	void SetViewAngles(float pitch, float yaw);
	void IsShooting();
	bool IsAiming();
	float DistanceFromCrosshair(Entity* ent);
	
	Entity* entity;
	Entity* closestToCrosshair;
	Entity* lockedEntity;
	Window* win;

	float closestDistToCrosshair;
	float aimSpeed = 0.25;
private:
	float aimProgress = 0;
	Vector3 aimTarget;
};

