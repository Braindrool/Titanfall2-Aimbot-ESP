#pragma once
#include "VectorStructs.h"

// World To Screen functions. Credit to https://guidedhacking.com/threads/world2screen-direct3d-and-opengl-worldtoscreen-functions.8044/
class WorldToScreen
{

public:
	static bool D3D(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
	static bool OpenGL(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
};