#pragma once

#include "Core/Message.h"

#include <string>
#include <vector>
#include <deque>
#include <functional>

using TokenCallback = std::function<void(const std::string &)>;

class IAIProvider
{
public:
    virtual ~IAIProvider() = default;

    virtual std::string GenerateResponse(const std::deque<Message> &history, TokenCallback callback) = 0;
};
