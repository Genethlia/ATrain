#include "CommandRouter.h"

CommandRouter::CommandRouter(ApplicationLauncher &launcher)
    : launcher(launcher)
{
}

bool CommandRouter::isCommandAndTurntoLowerCase(std::string &command)
{
    for (auto &x : command)
    {
        x = tolower(x);
    }
    return command.find("open ") != std::string::npos;
}

void CommandRouter::executeCommand(const std::string &command)
{
    std::string appName = command.substr(5); // Remove "open " from the beginning
    while (!appName.empty() && appName.front() == ' ')
    {
        appName.erase(0, 1); // Remove leading spaces
    }
    while (!appName.empty() && appName.back() == ' ')
    {
        appName.pop_back(); // Remove trailing spaces
    }
    launcher.OpenApplication(appName);
}
