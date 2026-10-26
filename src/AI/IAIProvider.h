#pragma once

#include "Core/Message.h"

#include <string>
#include <vector>
#include <deque>

class IAIProvider
{
public:
    virtual ~IAIProvider() = default;

    virtual std::string GenerateResponse(const std::deque<Message> &history, long long &tokenCount) = 0;
};
