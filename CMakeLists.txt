
cmake_minimum_required(VERSION 3.21)

project(ATrain VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(FetchContent)

# Dependency settings
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_TESTS_SSL OFF CACHE BOOL "" FORCE)
set(CPR_USE_SYSTEM_LIB_PSL ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)

# CPR HTTP library
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75
)

# JSON library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)

FetchContent_MakeAvailable(cpr json)

# Core library
add_library(atrain_core STATIC
    src/Core/Assistant.cpp
)

target_include_directories(atrain_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Ollama provider
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

# Main executable
add_executable(ATrain
    src/main.cpp
)

target_link_libraries(ATrain PRIVATE
    atrain_ollama
)

# Compiler warnings
function(atrain_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4 /permissive-
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic
        )
    endif()
endfunction()

atrain_warnings(atrain_core)
atrain_warnings(atrain_ollama)
atrain_warnings(ATrain)

# Copy Windows runtime DLLs
if(WIN32)
    add_custom_command(
        TARGET ATrain POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            "-DRUNTIME_DLLS=$<TARGET_RUNTIME_DLLS:ATrain>"
            "-DDESTINATION=$<TARGET_FILE_DIR:ATrain>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CopyRuntime.cmake"
        VERBATIM
    )
endif()