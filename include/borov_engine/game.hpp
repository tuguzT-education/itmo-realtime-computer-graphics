#pragma once

#ifndef BOROV_ENGINE_GAME_HPP_INCLUDED
#define BOROV_ENGINE_GAME_HPP_INCLUDED

#include <d3d11.h>
#include <wrl/client.h>

#include "game.fwd"
#include "window.hpp"

namespace borov_engine {

class Game {
    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

  public:
    explicit Game(Window &window);

    ComPtr<ID3D11RenderTargetView> render_target_view_;
    ComPtr<IDXGISwapChain> swap_chain_;
    ComPtr<ID3D11DeviceContext> device_context_;
    ComPtr<ID3D11Device> device_;

  private:
    void InitializeDevice();
    void InitializeSwapChain(const Window &window);
    void InitializeRenderTargetView();
};

}

#endif //BOROV_ENGINE_GAME_HPP_INCLUDED
