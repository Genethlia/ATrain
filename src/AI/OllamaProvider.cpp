#include "OllamaProvider.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <stdexcept>
#include <utility>

OllamaProvider::OllamaProvider(const std::string &model)
    : modelName(std::move(model))
{
}

std::string OllamaProvider::GenerateResponse(const std::deque<Message> &history)
{
    nlohmann::json request{
        {"model", modelName},
        {"stream", false},
        {"messages", nlohmann::json::array()},
    };

    for (const auto &msg : history)
        request["messages"].push_back(nlohmann::json{{"role", msg.role}, {"content", msg.content}});

    auto response = cpr::Post(
        cpr::Url{"http://localhost:11434/api/chat"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{request.dump()},
        cpr::Timeout{120000});

    if (response.error.code != cpr::ErrorCode::OK)
    {
        throw std::runtime_error("Request failed: " + response.error.message);
    }

    if (response.status_code != 200)
    {
        throw std::runtime_error("Ollama Error " + response.text);
    }

    auto result = nlohmann::json::parse(response.text);

    auto content = result.at("message").at("content").get<std::string>();

    return content;
}
