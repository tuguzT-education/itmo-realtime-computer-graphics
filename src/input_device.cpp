#include "borov_engine/input_device.hpp"

#include <iostream>
#include <stdexcept>
#include <array>

#include "borov_engine/detail/err_handling_api.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

namespace detail {

enum class MouseButtonFlags : std::uint16_t {
    None = 0,

    LeftButtonDown = 1,
    LeftButtonUp = 2,
    RightButtonDown = 4,
    RightButtonUp = 8,
    MiddleButtonDown = 16,
    MiddleButtonUp = 32,
    Button4Down = 64,
    Button4Up = 128,
    Button5Down = 256,
    Button5Up = 512,
    MouseWheel = 1024,
    MouseHWheel = 2048,

    Button1Down = LeftButtonDown,
    Button1Up = LeftButtonUp,
    Button2Down = RightButtonDown,
    Button2Up = RightButtonUp,
    Button3Down = MiddleButtonDown,
    Button3Up = MiddleButtonUp,
};

}

InputDevice::InputDevice(Window &window) : window_{window}, mouse_move_data_{} {
    std::array raw_input_devices{
        RAWINPUTDEVICE{
            .usUsagePage = 0x01,
            .usUsage = 0x02,
            .dwFlags = 0, // adds HID mouse and also ignores legacy mouse messages
            .hwndTarget = window.GetRawHandle(),
        },
        RAWINPUTDEVICE{
            .usUsagePage = 0x01,
            .usUsage = 0x06,
            .dwFlags = 0, // adds HID keyboard and also ignores legacy keyboard messages
            .hwndTarget = window.GetRawHandle(),
        },
    };

    if (!RegisterRawInputDevices(raw_input_devices.data(),
                                 raw_input_devices.size(),
                                 sizeof(decltype(raw_input_devices)::value_type))) {
        std::string message = detail::GetLastError();
        throw std::runtime_error{message};
    }

    assert(window.input_device_ == nullptr && "Input device already exists");
    window.input_device_ = this;
}

InputDevice::~InputDevice() {
    window_.input_device_ = nullptr;
}

bool InputDevice::IsKeyDown(InputKey key) const {
    return keys_.count(key) > 0;
}

auto InputDevice::GetMouseMoveData() const -> const MouseMoveData & {
    return mouse_move_data_;
}

auto InputDevice::GetMouseMoveData() -> MouseMoveData & {
    return mouse_move_data_;
}

auto InputDevice::GetOnMouseMove() const -> const OnMouseMove & {
    return on_mouse_move_;
}

auto InputDevice::GetOnMouseMove() -> OnMouseMove & {
    return on_mouse_move_;
}

auto InputDevice::GetOnInputKeyUp() const -> const OnInputKeyUp & {
    return on_input_key_up_;
}

auto InputDevice::GetOnInputKeyUp() -> OnInputKeyUp & {
    return on_input_key_up_;
}

auto InputDevice::GetOnInputKeyDown() const -> const OnInputKeyDown & {
    return on_input_key_up_;
}

auto InputDevice::GetOnInputKeyDown() -> OnInputKeyDown & {
    return on_input_key_up_;
}

void InputDevice::OnRawKeyboard(RawKeyboardData data) {
    bool a_break = data.flags & 0x01;

    auto key = static_cast<InputKey>(data.v_key);
    if (data.make_code == 42) {
        key = InputKey::LeftShift;
    } else if (data.make_code == 54) {
        key = InputKey::RightShift;
    }

    if (a_break) {
        if (IsKeyDown(key)) RemovePressedKey(key);
    } else {
        if (!IsKeyDown(key)) AddPressedKey(key);
    }
}

void InputDevice::OnRawMouse(RawMouseData data) {
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::LeftButtonDown)) {
        AddPressedKey(InputKey::LeftButton);
    }
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::LeftButtonUp)) {
        RemovePressedKey(InputKey::LeftButton);
    }
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::RightButtonDown)) {
        AddPressedKey(InputKey::RightButton);
    }
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::RightButtonUp)) {
        RemovePressedKey(InputKey::RightButton);
    }
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::MiddleButtonDown)) {
        AddPressedKey(InputKey::MiddleButton);
    }
    if (data.button_flags & static_cast<int>(detail::MouseButtonFlags::MiddleButtonUp)) {
        RemovePressedKey(InputKey::MiddleButton);
    }

    POINT point;
    ::GetCursorPos(&point);
    ::ScreenToClient(window_.GetRawHandle(), &point);

    mouse_move_data_ = {
        .position = {static_cast<float>(point.x), static_cast<float>(point.y)},
        .offset = {static_cast<float>(data.x), static_cast<float>(data.y)},
        .wheel_delta = data.wheel_delta,
    };
    on_mouse_move_.Broadcast(mouse_move_data_);
}

void InputDevice::AddPressedKey(InputKey key) {
    keys_.insert(key);
    on_input_key_down_.Broadcast(key);
}

void InputDevice::RemovePressedKey(InputKey key) {
    keys_.erase(key);
    on_input_key_up_.Broadcast(key);
}

}
