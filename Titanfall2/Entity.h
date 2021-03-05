#pragma once
#include "EntityData.h"
class Entity
{
public:
	bool IsLocalPlayer();
	bool IsPlayer();
	bool IsAlive();
	bool IsFriendly();
	bool IsTitan();
	bool IsAI();
	bool IsSafeToAccess();

	void SetLastPos(Vector3 &lastPos);

	Vector3 GetBone(const char* name);
	Vector3 GetAimPoint(const char* boneName, float multiplier);
	Vector3 GetVelocity();

	float GetDistanceFrom(Entity* ent);

	EntityData* data;

	// Set up the bones
	std::map<const char*, unsigned int> humanBones{
		{"head", 10},
		{"neck", 9},
		{"weapon?", 8},
		{"chest", 7},
		{"abdomen", 6},
		{"hips", 5},
		{"leftshoulder", 15},
		{"leftupperarm", 17},
		{"leftelbow", 16},
		{"leftforearm", 19},
		{"leftwrist", 20},
		{"rightshoulder", 38},
		{"rightupperarm", 41},
		{"rightelbow", 42},
		{"rightforearm", 43},
		{"rightwrist", 44},
		{"lefthip", 61},
		{"righthip", 66},
		{"leftknee", 65},
		{"rightknee", 70},
		{"leftankle", 63},
		{"rightankle", 68},
		{"leftfoot", 64},
		{"rightfoot", 69}
	};

private:
	Vector3 lastPos = { 0, 0, 0 };
	Vector3 velocity;
};
