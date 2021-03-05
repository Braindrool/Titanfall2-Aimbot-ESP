#pragma once
#include "Entity.h"

#define MAX_ENTITIES 600

class EntityList
{
public:
	void Refresh();
	void Cleanup();
	static std::map<unsigned int, Entity*> Entities;
private:
};