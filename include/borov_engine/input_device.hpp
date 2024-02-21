#pragma once

#ifndef BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED
#define BOROV_ENGINE_INPUT_DEVICE_HPP_INCLUDED

#include <directxmath.h>
#include <windows.h>

#include <unordered_set>

#include "window.hpp"
#include "input_key.hpp"
#include "delegate/multicast_delegate.hpp"

namespace borov_engine {

class InputDevice {
  public:
    struct MouseMoveData {
        DirectX::XMFLOAT2 position;
        DirectX::XMFLOAT2 offset;
        int wheel_delta;
    };

    DECLARE_MULTICAST_DELEGATE(OnMouseMove, const MouseMoveData &);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyUp, InputKey);
    DECLARE_MULTICAST_DELEGATE(OnInputKeyDown, InputKey);

    explicit InputDevice(Window &window);
    ~InputDevice();

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

    struct RawKeyboardData {
        USHORT make_code;
        USHORT flags;
        USHORT v_key;
        UINT message;
    };

    struct RawMouseData {
        /* MOUSE_MOVE_RELATIVE */
        int mode;
        int button_flags;
        int extra_information;
        int buttons;
        int wheel_delta;
        int x;
        int y;
    };

    void OnRawKeyboard(RawKeyboardData data);
    void OnRawMouse(RawMouseData data);

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
