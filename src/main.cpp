#ifdef _WIN32
#include <Windows.h>
#endif

#include "Core/Assistant.h"
#include "AI/OllamaProvider.h"
#include "Server/WebServer.h"
#include "System/ApplicationLauncher.h"
#include "Commands/CommandRouter.h"

#include <iostream>
#include <memory>
#include <string>
#include <exception>

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Set the console output code page to UTF-8
    SetConsoleCP(CP_UTF8);       // Set the console input code page to UTF-8
#endif
    Assistant ATrain(std::make_unique<OllamaProvider>("qwen3:4b")); // Initialize the Assistant with the OllamaProvider and the specified model
    ApplicationLauncher launcher;                                   // Create an instance of the ApplicationLauncher class
    CommandRouter router(launcher);                                 // Create an instance of the CommandRouter class with the ApplicationLauncher
    WebServer server = WebServer(ATrain, launcher, router);         // Create an instance of the WebServer class with the Assistant, ApplicationLauncher, and CommandRouter
    server.start();                                                 // Start the assistant
}