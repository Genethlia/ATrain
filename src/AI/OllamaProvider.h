#pragma once

#include "IAIProvider.h"
#include "Core/Message.h"

#include <string>
#include <vector>
#include <deque>
#include <functional>

class OllamaProvider : public IAIProvider
{
public:
    explicit OllamaProvider(const std::string &model);

    std::string GenerateResponse(const std::deque<Message> &history, TokenCallback callback) override;

private:
    std::string modelName;
};
