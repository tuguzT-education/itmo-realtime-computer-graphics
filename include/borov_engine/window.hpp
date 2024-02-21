#pragma once

#ifndef BOROV_ENGINE_WINDOW_HPP_INCLUDED
#define BOROV_ENGINE_WINDOW_HPP_INCLUDED

#include <windows.h>

namespace borov_engine {

class Window {
  public:
    struct Dimensions {
        LONG width;
        LONG height;
    };

    explicit Window(LPCTSTR name, LONG width, LONG height, HINSTANCE instanceHandle = nullptr);

    [[nodiscard]] HWND GetRawHandle() const;
    [[nodiscard]] HINSTANCE GetRawInstanceHandle() const;

    [[nodiscard]] RECT GetRect() const;
    [[nodiscard]] Dimensions GetDimensions() const;

    [[nodiscard]] RECT GetClientRect() const;
    [[nodiscard]] Dimensions GetClientDimensions() const;

    bool SetTitle(LPCTSTR title);

  private:
    HWND handle;
};

}

#endif //BOROV_ENGINE_WINDOW_HPP_INCLUDED
