#pragma once

#include "AI/IAIProvider.h"
#include "Core/Message.h"

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <deque>

class Assistant // The Assistant class manages the conversation history and interacts with the AI provider to generate responses
{
protected:
    size_t tokenCount = 0; // Tracks the number of tokens used in the conversation, including both user and assistant messages

private:
    std::unique_ptr<IAIProvider> provider; // Pointer to the AI provider, for example ollama

    std::deque<Message> history; // Stores the conversation history

    std::string historySummary; // Stores a summary of the conversation history

    size_t contextBudget = 16384; // Max Tokens
    size_t reservedTokens = 2048; // Reserve tokens for the model's response plus safety margin

    size_t EstimateTokens(const std::deque<Message> &messages) const; // Estimates the number of tokens roughly used in the conversation history

    void SummarizeHistory(); // Summarizes the conversation history to reduce token usage
    void TrimHistory();      // Trims the conversation history to fit within the context budget

    bool RemoveOldestCompleteTurn(); // Removes the oldest complete turn (user + assistant messages) from the conversation history

    bool CheckIfShouldSummarize();                   // Checks if the conversation history should be summarized
    std::deque<Message> BuildRequestHistory() const; // Builds the message history to send to the AI provider

    void AddMessage(const std::string &role, const std::string &content); // Adds a message to the conversation history and updates the token count

public:
    explicit Assistant(std::unique_ptr<IAIProvider> ai); // Constructor

    std::string GenerateReply(const std::string &message); // Returns the response

    void SetProvider(std::unique_ptr<IAIProvider> newProvider); // Sets a new AI provider

    void ClearHistory(); // Clears the conversation history
};