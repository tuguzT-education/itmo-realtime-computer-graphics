#include "borov_engine/input.hpp"

#include <hidusage.h>

#include <iostream>
#include <stdexcept>
#include <array>

#include "borov_engine/detail/err_handling_api.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

Input::Input(Window &window) : window_{window}, mouse_move_data_{} {
    std::array raw_input_devices{
        RAWINPUTDEVICE{
            .usUsagePage = HID_USAGE_PAGE_GENERIC,
            .usUsage = HID_USAGE_GENERIC_MOUSE,
            .dwFlags = 0, // adds HID mouse and also ignores legacy mouse messages
            .hwndTarget = window.RawHandle(),
        },
        RAWINPUTDEVICE{
            .usUsagePage = HID_USAGE_PAGE_GENERIC,
            .usUsage = HID_USAGE_GENERIC_KEYBOARD,
            .dwFlags = 0, // adds HID keyboard and also ignores legacy keyboard messages
            .hwndTarget = window.RawHandle(),
        },
    };

    if (!RegisterRawInputDevices(raw_input_devices.data(),
                                 raw_input_devices.size(),
                                 sizeof(decltype(raw_input_devices)::value_type))) {
        std::string message = detail::LastError();
        throw std::runtime_error{message};
    }

    assert(window.input_ == nullptr && "Input device already exists");
    window.input_ = this;
}

Input::~Input() {
    window_.input_ = nullptr;
}

bool Input::IsKeyDown(InputKey key) const {
    return keys_.count(key) > 0;
}

const MouseMoveData &Input::MouseMoveData() const {
    return mouse_move_data_;
}

borov_engine::MouseMoveData &Input::MouseMoveData() {
    return mouse_move_data_;
}

const OnMouseMove &Input::OnMouseMove() const {
    return on_mouse_move_;
}

OnMouseMove &Input::OnMouseMove() {
    return on_mouse_move_;
}

const OnInputKeyUp &Input::OnInputKeyUp() const {
    return on_input_key_up_;
}

OnInputKeyUp &Input::OnInputKeyUp() {
    return on_input_key_up_;
}

const OnInputKeyDown &Input::OnInputKeyDown() const {
    return on_input_key_down_;
}

OnInputKeyDown &Input::OnInputKeyDown() {
    return on_input_key_down_;
}

void Input::OnRawKeyboard(const RAWKEYBOARD &data) {
    bool is_key_up = data.Flags & RI_KEY_BREAK;

    auto key = static_cast<InputKey>(data.VKey);
    switch (data.MakeCode) {
        case 0x001D: {
            key = InputKey::LeftControl;
            break;
        }
        case 0x002A: {
            key = InputKey::LeftShift;
            break;
        }
        case 0x0038: {
            key = InputKey::LeftAlt;
            break;
        }
        case 0xE01D: {
            key = InputKey::RightControl;
            break;
        }
        case 0x0036: {
            key = InputKey::RightShift;
            break;
        }
        case 0xE038: {
            key = InputKey::RightAlt;
            break;
        }
    }

    if (is_key_up) {
        if (IsKeyDown(key)) RemovePressedKey(key);
    } else {
        if (!IsKeyDown(key)) AddPressedKey(key);
    }
}

void Input::OnRawMouse(const RAWMOUSE &data) {
    if (data.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
        AddPressedKey(InputKey::LeftButton);
    }
    if (data.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
        RemovePressedKey(InputKey::LeftButton);
    }
    if (data.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
        AddPressedKey(InputKey::RightButton);
    }
    if (data.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
        RemovePressedKey(InputKey::RightButton);
    }
    if (data.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
        AddPressedKey(InputKey::MiddleButton);
    }
    if (data.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
        RemovePressedKey(InputKey::MiddleButton);
    }

    POINT point;
    ::GetCursorPos(&point);
    ::ScreenToClient(window_.RawHandle(), &point);

    mouse_move_data_ = {
        .position = {static_cast<float>(point.x), static_cast<float>(point.y)},
        .offset = {static_cast<float>(data.lLastX), static_cast<float>(data.lLastY)},
        .wheel_delta = data.usButtonData,
    };
    on_mouse_move_.Broadcast(mouse_move_data_);
}

void Input::AddPressedKey(InputKey key) {
    keys_.insert(key);
    on_input_key_down_.Broadcast(key);
}

void Input::RemovePressedKey(InputKey key) {
    keys_.erase(key);
    on_input_key_up_.Broadcast(key);
}

}
