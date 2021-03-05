#include "pch.h"
#include "Utility.h"

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) {
    wchar_t buff[255];

    GetWindowText(hWnd, buff, 254);
    // if (wcscmp(buff, Utility::name) == 0) {
    std::cout << "Found" << std::endl;

    return false;
    // }
     //printf("%s\n", buff);

    return TRUE;
}
