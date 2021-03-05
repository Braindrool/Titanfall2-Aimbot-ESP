#pragma once

// Map names to addresses
class Addresses
{
public:
	static void Init();
	static uintptr_t Get(const char* name);
	static void Set(const char* name, uintptr_t address);

private:
	static std::map<const char*, uintptr_t> addressMap;
};

