#pragma once

#include "AI/IAIProvider.h"
#include "Core/Message.h"

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <deque>

class Assistant
{
protected:
    long long tokenCount = 0;

private:
    std::unique_ptr<IAIProvider> provider;
    std::deque<Message> history;

public:
    explicit Assistant(std::unique_ptr<IAIProvider> ai);

    std::string Ask(const std::string &message);

    void SetProvider(std::unique_ptr<IAIProvider> newProvider);

    void Summarize();
    void ShouldSummarize();

    void ClearHistory();
};