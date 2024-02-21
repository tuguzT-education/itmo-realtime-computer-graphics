#include <iostream>

#include <borov_engine/window.hpp>
#include <borov_engine/game.hpp>
#include <borov_engine/delegate/delegate.hpp>
#include <borov_engine/delegate/multicast_delegate.hpp>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main() {
    constexpr LONG width = 800, height = 800;
    borov_engine::Window window{TEXT("Application"), width, height, GetModuleHandle(nullptr)};
    borov_engine::Game game{window};

    game.Run();
    std::cout << "Hello World!\n";

    borov_engine::delegate::Delegate<int> delegate;
    delegate.BindLambda([] { return 42; });
    auto result = delegate.ExecuteIfBound();
    std::cout << result.value() << std::endl;

    borov_engine::delegate::MulticastDelegate multicast_delegate;
    multicast_delegate.AddLambda([&result] { std::cout << result.value() << std::endl; });
    multicast_delegate.Broadcast();

    return 0;
}
