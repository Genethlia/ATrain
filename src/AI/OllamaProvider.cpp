#include "OllamaProvider.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <stdexcept>
#include <iostream>
#include <utility>

OllamaProvider::OllamaProvider(const std::string &model)
    : modelName(std::move(model))
{
}

std::string OllamaProvider::GenerateResponse(const std::deque<Message> &history, TokenCallback callback)
{
    nlohmann::json request{
        {"model", modelName},
        {"stream", true},
        {"messages", nlohmann::json::array()},
    };

    for (const auto &msg : history)
        request["messages"].push_back(nlohmann::json{{"role", msg.role}, {"content", msg.content}});

    std::string buffer;
    std::string fullResponse;
    std::string streamError;
    bool done = false;

    auto response = cpr::Post(
        cpr::Url{"http://localhost:11434/api/chat"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{request.dump()},
        cpr::Timeout{120000},
        cpr::WriteCallback{
            [&](std::string_view data, intptr_t) -> bool
            {
                buffer.append(data.data(), data.size());

                size_t pos;

                while ((pos = buffer.find("\n")) != std::string::npos)
                {
                    std::string line = buffer.substr(0, pos);
                    buffer.erase(0, pos + 1);

                    if (line.empty())
                        continue;

                    try
                    {
                        auto json = nlohmann::json::parse(line);

                        if (json.contains("error"))
                        {
                            streamError = json.at("error").get<std::string>();
                        }

                        auto content = json.value("message", nlohmann::json::object()).value("content", "");
                        fullResponse += content;
                        callback(content);

                        if (json.value("done", false))
                        {
                            done = true;
                        }
                    }
                    catch (const std::exception &e)
                    {
                        streamError = e.what();
                        return false;
                    }
                }
                return true;
            }});

    if (!streamError.empty())
    {
        throw std::runtime_error("Ollama stream error: " + streamError);
    }

    if (response.error.code != cpr::ErrorCode::OK)
    {
        throw std::runtime_error("Request failed: " + response.error.message);
    }

    if (response.status_code != 200)
    {
        throw std::runtime_error("Ollama Error " + response.text);
    }

    if (!done)
    {
        throw std::runtime_error("Ollama response was not completed.");
    }

    return fullResponse;
}
