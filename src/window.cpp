#include "borov_engine/window.hpp"

#include <winuser.h>

namespace borov_engine {

namespace detail {

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    switch (uMessage) {
        case WM_KEYDOWN: {
            // If a key is pressed send it to the input object, so it can record that state.
            // std::cout << "Key: " << static_cast<unsigned int>(wParam) << std::endl;

            if (static_cast<unsigned int>(wParam) == 27) PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE: {
            DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default: {
            return DefWindowProc(hWnd, uMessage, wParam, lParam);
        }
    }
}

}

Window::Window(LPCTSTR name, LONG width, LONG height, HINSTANCE instanceHandle) {
    instanceHandle = (instanceHandle != nullptr) ? instanceHandle : GetModuleHandle(nullptr);

    WNDCLASSEX wc{
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = detail::WndProc,
        .hInstance = instanceHandle,
        .hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
        .lpszClassName = name,
    };
    RegisterClassEx(&wc);

    RECT rect{.left = 0, .top = 0, .right = width, .bottom = height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    handle = CreateWindowEx(WS_EX_APPWINDOW, name, name,
                            WS_OVERLAPPEDWINDOW,
                            (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
                            (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
                            rect.right - rect.left,
                            rect.bottom - rect.top,
                            nullptr, nullptr, instanceHandle, nullptr);

    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);
    ShowCursor(true);
}

HWND Window::GetRawHandle() const {
    return handle;
}

HINSTANCE Window::GetRawInstanceHandle() const {
    return (HINSTANCE) GetWindowLongPtr(handle, GWLP_HINSTANCE);
}

RECT Window::GetRect() const {
    RECT rect{};
    GetWindowRect(handle, &rect);
    return rect;
}

Dimensions Window::GetDimensions() const {
    RECT rect = GetRect();
    return Dimensions{
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

RECT Window::GetClientRect() const {
    RECT rect{};
    ::GetClientRect(handle, &rect);
    return rect;
}

Dimensions Window::GetClientDimensions() const {
    RECT rect = GetClientRect();
    return Dimensions{
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

bool Window::SetTitle(LPCTSTR title) {
    return SetWindowText(handle, title);
}

}
