#include <System/WindowManager.h>

void WindowManager::FocusWindowByTitle(const std::string &windowTitle)
{
    HWND hwnd = GetWindowHandleByTitle(windowTitle);
    if (hwnd)
    {
        SetForegroundWindow(hwnd);
    }
}

void WindowManager::MinimizeWindowByTitle(const std::string &windowTitle)
{
    HWND hwnd = GetWindowHandleByTitle(windowTitle);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_MINIMIZE);
    }
}

void WindowManager::MaximizeWindowByTitle(const std::string &windowTitle)
{
    HWND hwnd = GetWindowHandleByTitle(windowTitle);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
}

void WindowManager::RestoreWindowByTitle(const std::string &windowTitle)
{
    HWND hwnd = GetWindowHandleByTitle(windowTitle);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_RESTORE);
    }
}

void WindowManager::CloseWindowByTitle(const std::string &windowTitle)
{
    HWND hwnd = GetWindowHandleByTitle(windowTitle);
    if (hwnd)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}

BOOL CALLBACK WindowManager::FindCallback(HWND hwnd, LPARAM lParam)
{
    WindowSearch *search = reinterpret_cast<WindowSearch *>(lParam);

    if (!IsWindowVisible(hwnd))
    {
        return TRUE;
    }

    wchar_t windowTitle[256] = {};
    GetWindowTextW(hwnd, windowTitle, 256);

    if (std::wstring(windowTitle).find(search->title) != std::wstring::npos)
    {
        search->hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

HWND WindowManager::GetWindowHandleByTitle(const std::string &windowTitle)
{
    WindowSearch search;
    search.title = std::wstring(windowTitle.begin(), windowTitle.end());

    EnumWindows(WindowManager::FindCallback, reinterpret_cast<LPARAM>(&search));
    return search.hwnd;
}