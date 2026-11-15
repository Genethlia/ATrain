#pragma once
#include <windows.h>

inline void SimulateTabKeyPress() // Simulate a Tab key press and release, for reseting keyboard focus on Windows
{
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_TAB;
    SendInput(1, &input, sizeof(INPUT)); // Simulate a Tab key press

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT)); // Simulate a Tab key release
}