# Core library.
add_library(atrain_core STATIC
    src/Core/Assistant.cpp
)

target_include_directories(atrain_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Ollama provider.
add_library(atrain_ollama STATIC
    src/AI/OllamaProvider.cpp
)

target_link_libraries(atrain_ollama
    PUBLIC
        atrain_core
    PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)

# Main executable.
add_executable(ATrain
    src/main.cpp
    src/Server/WebServer.cpp
    src/System/ApplicationLauncher.cpp
    src/Commands/CommandRouter.cpp
    src/System/WindowManager.cpp
)

target_link_libraries(ATrain PRIVATE
    atrain_ollama
    Crow::Crow
    ws2_32
    mswsock
    ole32
    shell32
)

target_compile_definitions(ATrain PRIVATE
    WIN32_LEAN_AND_MEAN
)

atrain_warnings(atrain_core)
atrain_warnings(atrain_ollama)
atrain_warnings(ATrain)
