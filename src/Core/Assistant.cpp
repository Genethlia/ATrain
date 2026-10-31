#include "Core/Assistant.h"

#include <stdexcept>
#include <utility>

size_t Assistant::EstimateTokens(const std::deque<Message> &messages) const
{
    size_t totalTokens = 0;
    for (const auto &msg : messages)
    {
        totalTokens += msg.content.size() / 4; // 1 token is almost 4 characters
    }
    return totalTokens;
}

bool Assistant::CheckIfShouldSummarize()
{
    const size_t availableTokens = contextBudget - reservedTokens; // Calculate available tokens for conversation history
    return tokenCount > availableTokens * 3 / 4;                   // Summarize if token count exceeds 75% of available tokens
}

std::deque<Message> Assistant::BuildRequestHistory() const
{
    std::deque<Message> request;

    if (!historySummary.empty())
    {
        request.push_back({"system", "Relevent conversation memory\n: " + historySummary});
    }

    for (const auto &msg : history)
    {
        request.push_back(msg);
    }
    return request;
}

void Assistant::SummarizeHistory()
{
    std::deque<Message> oldMessages;
    size_t messagesToSummarize = history.size() / 2; // Summarize half of the conversation history
    messagesToSummarize -= messagesToSummarize % 2;  // Ensure an even number of messages to summarize

    for (size_t i = 0; i < messagesToSummarize; ++i)
    {
        oldMessages.push_back(history[i]);
    }

    std::deque<Message> summaryRequest;
    summaryRequest.push_back({"system", summaryPrompt});
    summaryRequest.push_back({"system", "Existing summary: " + historySummary + "\n\n Update the summary using the following conversation messages:"});

    for (const auto &msg : oldMessages)
    {
        summaryRequest.push_back(msg);
    }

    auto summary = provider->GenerateResponse(summaryRequest, [](const std::string &token) {});

    if (summary.empty())
    {
        throw std::runtime_error("Failed to generate summary.");
    }

    for (size_t i = 0; i < messagesToSummarize; ++i)
    {
        TrimHistory();
    }

    historySummary = std::move(summary);
    tokenCount = EstimateTokens(BuildRequestHistory()); // Update token count after summarization
}

void Assistant::TrimHistory()
{
    if (history.empty())
        return;

    history.pop_front();

    tokenCount = EstimateTokens(BuildRequestHistory()); // Update token count after trimming
}

bool Assistant::RemoveOldestCompleteTurn()
{
    if (history.size() < 3)
        return false;

    if (history[0].role != "user" || history[1].role != "assistant")
        return false;

    history.pop_front();
    history.pop_front();

    tokenCount = EstimateTokens(BuildRequestHistory()); // Update token count after removing a turn
    return true;
}

void Assistant::AddMessage(const std::string &role, const std::string &content)
{
    history.push_back({role, content});
    tokenCount = EstimateTokens(BuildRequestHistory()); // Update token count after adding a new message
}

Assistant::Assistant(std::unique_ptr<IAIProvider> ai) : provider(std::move(ai))
{
    if (!provider)
    {
        throw std::invalid_argument("AI provider cannot be null.");
    }
}

std::string Assistant::GenerateReply(const std::string &message)
{
    if (!provider)
    {
        throw std::runtime_error("No AI provider set.");
    }
    bool userMessageAdded = false;

    try
    {

        AddMessage("user", message);
        userMessageAdded = true;

        if (CheckIfShouldSummarize())
        {
            SummarizeHistory();
        }
        while (EstimateTokens(BuildRequestHistory()) > (contextBudget - reservedTokens))
        {
            if (!RemoveOldestCompleteTurn())
            {
                throw std::runtime_error("Cannot trim history further to fit within context budget.");
            }
        }

        auto response = provider->GenerateResponse(BuildRequestHistory(), [](const std::string &token)
                                                   { std::cout << token << std::flush; });
        AddMessage("assistant", response);

        std::cout << "\nToken count for this response: " << tokenCount << std::endl;

        return response;
    }

    catch (const std::exception &e)
    {
        std::cerr << "Error in GenerateReply: " << e.what() << std::endl;
        if (userMessageAdded && !history.empty() && history.back().role == "user" && history.back().content == message)
        {
            history.pop_back();
        }
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

void Assistant::ClearHistory()
{
    history.clear();
    tokenCount = 0;
    historySummary.clear();
}
