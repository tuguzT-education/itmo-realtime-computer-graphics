#pragma once

#ifndef BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED
#define BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED

#include <SimpleMath.h>

#include <unordered_set>

#include "window.hpp"
#include "input_key.hpp"
#include "delegate/multicast_delegate.hpp"

namespace borov_engine {

class Input {
  public:
    struct MouseMoveData {
        DirectX::SimpleMath::Vector2 position;
        DirectX::SimpleMath::Vector2 offset;
        int wheel_delta;
    };

    DECLARE_MULTICAST_DELEGATE(OnMouseMove, const MouseMoveData &);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyUp, InputKey);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyDown, InputKey);

    explicit Input(Window &window);
    ~Input();

    [[nodiscard]] bool IsKeyDown(InputKey key) const;

    [[nodiscard]] const MouseMoveData &GetMouseMoveData() const;
    [[nodiscard]] MouseMoveData &GetMouseMoveData();

    [[nodiscard]] const OnMouseMove &GetOnMouseMove() const;
    [[nodiscard]] OnMouseMove &GetOnMouseMove();

    [[nodiscard]] const OnInputKeyUp &GetOnInputKeyUp() const;
    [[nodiscard]] OnInputKeyUp &GetOnInputKeyUp();

    [[nodiscard]] const OnInputKeyDown &GetOnInputKeyDown() const;
    [[nodiscard]] OnInputKeyDown &GetOnInputKeyDown();

  private:
    friend class Window;

    void OnRawKeyboard(const RAWKEYBOARD &data);
    void OnRawMouse(const RAWMOUSE &data);

    void AddPressedKey(InputKey key);
    void RemovePressedKey(InputKey key);

    Window &window_;
    MouseMoveData mouse_move_data_;
    OnMouseMove on_mouse_move_;
    OnInputKeyDown on_input_key_down_;
    OnInputKeyUp on_input_key_up_;
    std::unordered_set<InputKey> keys_;
};

}

#endif //BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED
