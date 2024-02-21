#include <borov_engine/window.hpp>
#include <borov_engine/game.hpp>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main() {
    borov_engine::Window window{TEXT("Application"), 800, 800, GetModuleHandle(nullptr)};
    borov_engine::Game game{window};

    game.Run();
    return 0;
}
