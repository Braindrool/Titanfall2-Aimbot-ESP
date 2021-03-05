#include "pch.h"
#include "Addresses.h"

std::map<const char*, uintptr_t> Addresses::addressMap;

void Addresses::Init()
{
	// Initialize addresses
	Addresses::Set("client", (uintptr_t)GetModuleHandle(L"client.dll"));
	Addresses::Set("engine", (uintptr_t)GetModuleHandle(L"engine.dll"));
	Addresses::Set("localplayer", *(uintptr_t*)(Addresses::Get("client") + 0xB18748) + 0x18);// 0xE69EA0));
	Addresses::Set("entitylist", *(uintptr_t*)(Addresses::Get("engine") + 0xF849680));// 0xAEF950));
	Addresses::Set("traceline", (Addresses::Get("engine") + 0x14F7A0));
	Addresses::Set("inventory", *(uintptr_t*)(Addresses::Get("client") + 0x23F9F60));// 0xAEF950));
	//Addresses::Set("primaryweapon", *(uintptr_t*)(*(uintptr_t*)(Addresses::Get("client") + 0x23F9F60) + 0x20));// 0xAEF950));
	Addresses::Set("viewmatrix", (Addresses::Get("client") + 0xB221E0));// +0x22ABD68 + 0x58));
	Addresses::Set("camerapos", (Addresses::Get("client") + 0x11ED5B0));
	Addresses::Set("pitch", (Addresses::Get("client") + 0x11ED5B8)); // Needed?
	Addresses::Set("yaw", (Addresses::Get("client") + 0x11ED5B8)); // Needed?
}

uintptr_t Addresses::Get(const char* name)
{
	return addressMap.find(name)->second;
}

void Addresses::Set(const char* name, uintptr_t address)
{
	addressMap.insert({ name, address });
}
