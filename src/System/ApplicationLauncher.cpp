#include "ApplicationLauncher.h"

#include <System/SystemLibrary.h>
#include <objbase.h>
#include <shellapi.h>

ApplicationLauncher::ApplicationLauncher()
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

ApplicationLauncher::~ApplicationLauncher()
{
    CoUninitialize();
}

bool ApplicationLauncher::OpenApplication(const std::string &name)
{
    if (name.empty())
    {
        return false; // Return false if the application name is empty
    }
    auto it = applicationPaths.find(name);
    if (it != applicationPaths.end())
    {
        SimulateTabKeyPress();
        HINSTANCE hInstance = ShellExecute(nullptr, "open", it->second.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return reinterpret_cast<INT_PTR>(hInstance) > 32;
    }
    return false;
}