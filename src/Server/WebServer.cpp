#include "WebServer.h"
using namespace crow;

WebServer::WebServer(Assistant &assistant, ApplicationLauncher &launcher, CommandRouter &router)
    : assistant(assistant), launcher(launcher), router(router)
{
}

void WebServer::start()
{
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("http://127.0.0.1:5500")
        .methods(crow::HTTPMethod::POST, crow::HTTPMethod::GET, crow::HTTPMethod::OPTIONS)
        .headers("Content-Type");

    CROW_ROUTE(app, "/chat")
        .methods(crow::HTTPMethod::POST)([this](const crow::request &req)
                                         {
        auto data = crow::json::load(req.body); 
        
        if(!data||!data.has("message")||data["message"].t()!=crow::json::type::String){
            return crow::response(400, "Invalid request");
        }

        std::string message = data["message"].s();

        std::string answer;
        {
            std::lock_guard<std::mutex> lock(assistantMutex);
            if (router.isCommandAndTurntoLowerCase(message)) {
                router.executeCommand(message);
                answer = "Command executed: " + message;
            } else {
                answer = assistant.GenerateReply(message);
            }
        }

        crow::json::wvalue response;
        response["answer"] = answer;
        return crow::response(200, response.dump()); });

    app.port(8080).multithreaded().run();
}
