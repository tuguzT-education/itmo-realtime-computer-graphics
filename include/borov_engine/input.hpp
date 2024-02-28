#pragma once

#ifndef BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED
#define BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED

#include <unordered_set>

#include "window.hpp"
#include "input_key.hpp"
#include "delegate/multicast_delegate.hpp"

namespace borov_engine {

struct MouseMoveData {
    math::Vector2 position;
    math::Vector2 offset;
    int wheel_delta;
};

DECLARE_MULTICAST_DELEGATE(OnMouseMove, const MouseMoveData &);
DECLARE_MULTICAST_DELEGATE(OnInputKeyUp, InputKey);
DECLARE_MULTICAST_DELEGATE(OnInputKeyDown, InputKey);

class Input {
  public:
    explicit Input(Window &window);
    ~Input();

    [[nodiscard]] bool IsKeyDown(InputKey key) const;

    [[nodiscard]] const borov_engine::MouseMoveData &MouseMoveData() const;
    [[nodiscard]] borov_engine::MouseMoveData &MouseMoveData();

    [[nodiscard]] const borov_engine::OnMouseMove &OnMouseMove() const;
    [[nodiscard]] borov_engine::OnMouseMove &OnMouseMove();

    [[nodiscard]] const borov_engine::OnInputKeyUp &OnInputKeyUp() const;
    [[nodiscard]] borov_engine::OnInputKeyUp &OnInputKeyUp();

    [[nodiscard]] const borov_engine::OnInputKeyDown &OnInputKeyDown() const;
    [[nodiscard]] borov_engine::OnInputKeyDown &OnInputKeyDown();

  private:
    friend class Window;

    void OnRawKeyboard(const RAWKEYBOARD &data);
    void OnRawMouse(const RAWMOUSE &data);

    void AddPressedKey(InputKey key);
    void RemovePressedKey(InputKey key);

    Window &window_;
    borov_engine::MouseMoveData mouse_move_data_;
    borov_engine::OnMouseMove on_mouse_move_;
    borov_engine::OnInputKeyDown on_input_key_down_;
    borov_engine::OnInputKeyUp on_input_key_up_;
    std::unordered_set<InputKey> keys_;
};

}

#endif //BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED
