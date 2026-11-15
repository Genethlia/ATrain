#pragma once
#include <System/SystemLibrary.h>
#include <string>

struct WindowSearch
{
    std::wstring title;
    HWND hwnd = nullptr;
};

class WindowManager
{
public:
    WindowManager() = default;
    ~WindowManager() = default;

    void FocusWindowByTitle(const std::string &windowTitle);    // Focuses a window by its title, bringing it to the foreground
    void MinimizeWindowByTitle(const std::string &windowTitle); // Minimizes a window by its title
    void MaximizeWindowByTitle(const std::string &windowTitle); // Maximizes a window by its title
    void RestoreWindowByTitle(const std::string &windowTitle);  // Restores a minimized window by its title
    void CloseWindowByTitle(const std::string &windowTitle);    // Closes a window by its title

private:
    static BOOL CALLBACK FindCallback(HWND hwnd, LPARAM lParam); // Callback function for EnumWindows
    HWND GetWindowHandleByTitle(const std::string &windowTitle); // Retrieves the window handle (HWND) for a given window title
};