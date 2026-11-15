#pragma once
#include "System/ApplicationLauncher.h"
#include <string>

class CommandRouter // The CommandRouter class is responsible for routing commands to the appropriate handlers
{
public:
    CommandRouter(ApplicationLauncher &launcher); // Constructor that takes a reference to an ApplicationLauncher instance
    bool isCommandAndTurntoLowerCase(std::string &command);
    void executeCommand(const std::string &command);

private:
    ApplicationLauncher &launcher; // Reference to the ApplicationLauncher instance
};