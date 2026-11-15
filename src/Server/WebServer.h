#pragma once

#include "Core/Assistant.h"
#include "System/ApplicationLauncher.h"
#include "Commands/CommandRouter.h"

#include <crow.h>
#include <crow/middlewares/cors.h>
#include <mutex>

class WebServer // The WebServer class handles incoming HTTP requests and interacts with the Assistant to generate responses
{
public:
    explicit WebServer(Assistant &assistant, ApplicationLauncher &launcher, CommandRouter &router); // Constructor that takes a reference to an Assistant instance
    void start();                                                                                   // Starts the web server and listens for incoming requests

private:
    crow::App<crow::CORSHandler> app; // Crow application with CORS middleware
    Assistant &assistant;             // Reference to the Assistant instance
    ApplicationLauncher &launcher;    // Reference to the ApplicationLauncher instance
    CommandRouter &router;            // Reference to the CommandRouter instance
    std::mutex assistantMutex;        // Mutex to protect access to the Assistant instance
};