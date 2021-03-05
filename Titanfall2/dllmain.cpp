// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Titanfall.h"
#include "D3D.h"

Titanfall* tf;
bool safeToExit = false; // Threadsafe exit

void RenderLoop() {
	while (!GetAsyncKeyState(VK_END)) {
		tf->Rerender();
		Sleep(75);
	}
	safeToExit = true;
	ExitThread(0);
}


DWORD WINAPI MainThread(HMODULE hModule) {
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	tf = new Titanfall(hModule);

	// Start the render trhead
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RenderLoop, NULL, 0, nullptr);

	// Main loop
	while (!safeToExit) {
		tf->Run();
		tf->AimTick();
		Sleep(5);
	}

	
	tf->Cleanup();

	// Close the console and exit
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));

	return TRUE;
}