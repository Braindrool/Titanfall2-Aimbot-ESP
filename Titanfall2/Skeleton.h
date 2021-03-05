#pragma once
#include "Entity.h"
#include "WorldToScreen.h"
#include "Window.h"

class Skeleton
{
public:
	static void DrawSkeleton(Entity* ent);
	static void DrawBone(Entity* ent, const char* bone1, const char* bone2);
	static void DrawBoneNumbers(Entity* ent);
	static void SetWindow(Window* win);

private:
	static Window* win;
};

