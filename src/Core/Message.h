#pragma once

#include <string>

struct Message
{
    std::string role;
    std::string content;
};

inline const std::string summaryPrompt = "You maintain the memory of a personal AI assistant. Update the existing summary using the conversation messages provided. Preserve : -Important facts about the user -Project details and decisions - Preferences and requirements - Unfinished tasks - Important technical context. Remove : -Greetings -Repeated information -Unimportant details. Keep the summary concise and factual.Do not invent information. Return only the updated summary.";