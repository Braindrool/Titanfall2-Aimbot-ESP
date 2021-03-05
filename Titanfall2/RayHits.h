#pragma once
#include <xmmintrin.h>

class RayHits {
public:
	__m128 arrays[7];
	float data[1024];
};