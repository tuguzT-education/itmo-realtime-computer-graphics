#pragma once

#ifndef BOROV_ENGINE_WINDOW_HPP_INCLUDED
#define BOROV_ENGINE_WINDOW_HPP_INCLUDED

#include <windows.h>

#include <string_view>

namespace borov_engine {

class Window {
  public:
    struct Dimensions {
        LONG width;
        LONG height;
    };

    explicit Window(std::string_view name, LONG width, LONG height, HINSTANCE instance_handle = nullptr);

    [[nodiscard]] HWND GetRawHandle() const;
    [[nodiscard]] HINSTANCE GetRawInstanceHandle() const;

    [[nodiscard]] RECT GetRect() const;
    [[nodiscard]] Dimensions GetDimensions() const;

    [[nodiscard]] RECT GetClientRect() const;
    [[nodiscard]] Dimensions GetClientDimensions() const;

    bool SetTitle(std::string_view title);
    void Destroy();

  private:
    friend class InputDevice;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT u_message, WPARAM w_param, LPARAM l_param);

    HWND handle_;
    InputDevice *input_device_;
};

}

#endif //BOROV_ENGINE_WINDOW_HPP_INCLUDED
