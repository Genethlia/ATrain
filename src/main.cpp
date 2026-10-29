#ifdef _WIN32
#include <Windows.h>
#endif

#include "Core/Assistant.h"
#include "AI/OllamaProvider.h"

#include <iostream>
#include <memory>
#include <string>
#include <exception>

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Assistant ATrain(std::make_unique<OllamaProvider>("qwen3:4b"));

    std::cout << "A.T.R.A.I.N. initialized\n";
    std::cout << "Type /exit to quit\n\n";

    std::string input;
    while (true)
    {
        std::cout << "You: ";

        if (!std::getline(std::cin, input))
            break;

        if (input == "/exit")
            break;

        if (input.empty())
            continue;

        try
        {
            std::cout << "\nA.T.R.A.I.N.: " << ATrain.GenerateReply(input) << "\n\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}
