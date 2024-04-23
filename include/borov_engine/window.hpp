#pragma once

#ifndef BOROV_ENGINE_WINDOW_HPP_INCLUDED
#define BOROV_ENGINE_WINDOW_HPP_INCLUDED

#include <windows.h>

#include <string>

#include "delegate/multicast_delegate.hpp"
#include "math.hpp"

namespace borov_engine {

struct WindowResizeData {
    std::int32_t width;
    std::int32_t height;
};

DECLARE_MULTICAST_DELEGATE(OnWindowResize, WindowResizeData);

class Window {
  public:
    explicit Window(std::string_view name, std::int32_t width, std::int32_t height, std::int32_t min_width = 400,
                    std::int32_t min_height = 400, HINSTANCE instance_handle = nullptr);
    ~Window();

    [[nodiscard]] HWND RawHandle() const;
    [[nodiscard]] HINSTANCE RawInstanceHandle() const;

    [[nodiscard]] math::Rectangle Dimensions() const;
    [[nodiscard]] math::Rectangle ClientDimensions() const;

    [[nodiscard]] std::int32_t MinWidth() const;
    [[nodiscard]] std::int32_t &MinWidth();

    [[nodiscard]] std::int32_t MinHeight() const;
    [[nodiscard]] std::int32_t &MinHeight();

    [[nodiscard]] math::Point CursorPosition() const;
    bool CursorPosition(math::Point cursor_position);

    [[nodiscard]] bool IsDestroyed() const;
    [[nodiscard]] bool IsFocused() const;

    [[nodiscard]] std::string Title() const;
    bool Title(std::string_view title);

    [[nodiscard]] const OnWindowResize &OnResize() const;
    [[nodiscard]] OnWindowResize &OnResize();

    void ProcessQueueMessages();
    void Destroy();

  private:
    friend class Input;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT u_message, WPARAM w_param, LPARAM l_param);

    HWND handle_;
    std::int32_t min_width_;
    std::int32_t min_height_;
    bool is_destroyed_;
    Input *input_;
    OnWindowResize on_resize_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_WINDOW_HPP_INCLUDED
