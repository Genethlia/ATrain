#include "Core/Assistant.h"

#include <stdexcept>
#include <utility>

Assistant::Assistant(std::unique_ptr<IAIProvider> ai) : provider(std::move(ai))
{
    if (!provider)
    {
        throw std::invalid_argument("AI provider cannot be null.");
    }
}

std::string Assistant::Ask(const std::string &message)
{
    if (!provider)
    {
        throw std::runtime_error("No AI provider set.");
    }

    try
    {
        history.push_back({"user", message});

        auto response = provider->GenerateResponse(history, tokenCount);
        history.push_back({"assistant", response});

        std::cout << "Token count for this response: " << tokenCount << std::endl;

        return response;
    }

    catch (const std::exception &e)
    {
        history.pop_back();
        throw;
    }
}

void Assistant::SetProvider(std::unique_ptr<IAIProvider> newProvider)
{
    if (!newProvider)
    {
        throw std::invalid_argument("New AI provider cannot be null.");
    }

    provider = std::move(newProvider);
}

void Assistant::Summarize()
{
    std::string summaryPrompt = "";
    Ask(summaryPrompt);
    while (history.size() >= 8)
    {
        history.pop_front();
    }
    Message mess = history[history.size() - 1];
    history.push_front({"assistant", "Summary: " + mess.content});
    history.pop_back();
}

void Assistant::ShouldSummarize()
{
    if (tokenCount > 32168)
    {
        Summarize();
    }
}

void Assistant::ClearHistory()
{
    history.clear();
    tokenCount = 0;
}
