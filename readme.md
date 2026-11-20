# ATrain

ATrain is a Windows assistant written in C++20. It provides a browser-based chat interface powered by a local Ollama model and accepts commands such as `open notepad` and `open calculator` to launch Windows applications.

## Prerequisites

- Windows with the MSYS2 MinGW64 C++ toolchain and standalone Asio headers. The commands below assume MSYS2 is installed at `C:\msys64`.
- CMake 3.21 or later and Git. On the first configure, CMake downloads Crow, CPR, and nlohmann/json.
- Ollama for chat and Python 3 to serve the web interface.

If you need the toolchain and Asio, run this in the **MSYS2 MinGW64** shell:

```sh
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-asio
```

## Build

From the repository root in PowerShell:

```powershell
$env:Path = "C:\msys64\mingw64\bin;$env:Path"
cmake -S . -B build -G "MinGW Makefiles" -DASIO_INCLUDE_DIR=C:/msys64/mingw64/include
cmake --build build --target ATrain
```

## Run

1. Start Ollama if it is not already running, then download the default model:

   ```powershell
   ollama pull qwen3:4b
   ```

2. In the PowerShell session used to build ATrain, start the backend:

   ```powershell
   .\build\ATrain.exe
   ```

3. In a second terminal at the repository root, serve the web interface:

   ```powershell
   python -m http.server 5500 --bind 127.0.0.1 --directory web
   ```

4. Open `http://127.0.0.1:5500` in your browser. Use that exact address because the backend allows cross-origin requests from that origin.

The browser sends chat requests to `http://localhost:8080/chat`. The backend sends model requests to Ollama at `http://localhost:11434/api/chat`.

## Source layout

- [`src/Core/`](src/Core/) manages conversation history and summaries.
- [`src/AI/`](src/AI/) defines the model provider interface and implements the Ollama client.
- [`src/Server/`](src/Server/) handles browser requests through Crow.
- [`src/Commands/`](src/Commands/) and [`src/System/`](src/System/) handle commands and Windows integration. The application names and paths are defined in [`ApplicationLauncher.h`](src/System/ApplicationLauncher.h). Update the Spotify path there if your installation uses a different location.
- [`web/`](web/) contains the chat interface.
