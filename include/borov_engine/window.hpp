#pragma once

#ifndef BOROV_ENGINE_WINDOW_HPP_INCLUDED
#define BOROV_ENGINE_WINDOW_HPP_INCLUDED

#include <windows.h>

#include <string>

#include "math.hpp"

namespace borov_engine {

class Window {
  public:
    explicit Window(std::string_view name, LONG width, LONG height, HINSTANCE instance_handle = nullptr);
    ~Window();

    [[nodiscard]] HWND RawHandle() const;
    [[nodiscard]] HINSTANCE RawInstanceHandle() const;

    [[nodiscard]] math::Rectangle Dimensions() const;
    [[nodiscard]] math::Rectangle ClientDimensions() const;

    [[nodiscard]] bool IsDestroyed() const;
    [[nodiscard]] bool IsFocused() const;

    [[nodiscard]] std::string Title() const;
    bool Title(std::string_view title);

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
