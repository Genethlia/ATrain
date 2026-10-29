#pragma once

#include "IAIProvider.h"
#include "Core/Message.h"

#include <string>
#include <vector>
#include <deque>

class OllamaProvider : public IAIProvider
{
public:
    explicit OllamaProvider(const std::string &model);

    std::string GenerateResponse(const std::deque<Message> &history) override;

private:
    std::string modelName;
};
