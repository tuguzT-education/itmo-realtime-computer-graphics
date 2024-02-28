#pragma once

#ifndef BOROV_ENGINE_WINDOW_HPP_INCLUDED
#define BOROV_ENGINE_WINDOW_HPP_INCLUDED

#include <windows.h>

#include <string_view>

#include "math.hpp"

namespace borov_engine {

class Window {
  public:
    explicit Window(std::string_view name, LONG width, LONG height, HINSTANCE instance_handle = nullptr);
    ~Window();

    [[nodiscard]] HWND GetRawHandle() const;
    [[nodiscard]] HINSTANCE GetRawInstanceHandle() const;

    [[nodiscard]] Rectangle GetDimensions() const;
    [[nodiscard]] Rectangle GetClientDimensions() const;

    [[nodiscard]] bool IsDestroyed() const;
    [[nodiscard]] bool IsFocused() const;

    bool SetTitle(std::string_view title);
    void ProcessQueueMessages();
    void Destroy();

  private:
    friend class Input;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT u_message, WPARAM w_param, LPARAM l_param);

    HWND handle_;
    Input *input_;
    bool is_destroyed_;
};

}

#endif //BOROV_ENGINE_WINDOW_HPP_INCLUDED
