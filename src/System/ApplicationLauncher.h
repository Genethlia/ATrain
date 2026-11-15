#pragma once
#include <string>
#include <unordered_map>

class ApplicationLauncher
{
public:
    explicit ApplicationLauncher();                // Constructor for the ApplicationLauncher class
    ~ApplicationLauncher();                        // Destructor for the ApplicationLauncher class
    bool OpenApplication(const std::string &name); // Opens an application by name and returns true if successful, false otherwise

private:
    std::unordered_map<std::string, std::string> applicationPaths = {
        {"notepad", "notepad.exe"},
        {"calculator", "calc.exe"},
        {"spotify", "C:\\Users\\tgene\\AppData\\Roaming\\Spotify\\Spotify.exe"}};
};
