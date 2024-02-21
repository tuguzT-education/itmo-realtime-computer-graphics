#include "borov_engine/window.hpp"

#include <stdexcept>
#include <vector>

#include "borov_engine/input_device.hpp"

namespace borov_engine {

Window::Window(LPCTSTR name, LONG width, LONG height, HINSTANCE instance_handle) : input_device_{} {
    instance_handle = (instance_handle != nullptr)
                      ? instance_handle
                      : GetModuleHandle(nullptr);

    WNDCLASSEX wc{
        .cbSize = sizeof(decltype(wc)),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WndProc,
        .hInstance = instance_handle,
        .hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
        .lpszClassName = name,
    };
    RegisterClassEx(&wc);

    RECT rect{
        .right = width,
        .bottom = height,
    };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    handle_ = CreateWindowEx(WS_EX_APPWINDOW,
                             name,
                             name,
                             WS_OVERLAPPEDWINDOW,
                             (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
                             (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
                             rect.right - rect.left,
                             rect.bottom - rect.top,
                             nullptr,
                             nullptr,
                             instance_handle,
                             this);

    ShowWindow(handle_, SW_SHOW);
    SetForegroundWindow(handle_);
    SetFocus(handle_);
    ShowCursor(true);
}

HWND Window::GetRawHandle() const {
    return handle_;
}

HINSTANCE Window::GetRawInstanceHandle() const {
    return (HINSTANCE) GetWindowLongPtr(handle_, GWLP_HINSTANCE);
}

RECT Window::GetRect() const {
    RECT rect{};
    GetWindowRect(handle_, &rect);
    return rect;
}

auto Window::GetDimensions() const -> Dimensions {
    RECT rect = GetRect();
    return {
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

RECT Window::GetClientRect() const {
    RECT rect{};
    ::GetClientRect(handle_, &rect);
    return rect;
}

auto Window::GetClientDimensions() const -> Dimensions {
    RECT rect = GetClientRect();
    return {
        .width = rect.right - rect.left,
        .height = rect.bottom - rect.top,
    };
}

bool Window::SetTitle(LPCTSTR title) {
    return SetWindowText(handle_, title);
}

void Window::Destroy() {
    DestroyWindow(handle_);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT u_message, WPARAM w_param, LPARAM l_param) {
    Window *window;
    if (u_message == WM_NCCREATE) {
        auto create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
        window = static_cast<Window *>(create_struct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (!window) {
        return DefWindowProc(hwnd, u_message, w_param, l_param);
    }

    switch (u_message) {
        case WM_INPUT: {
            InputDevice *input_device = window->input_device_;
            if (input_device == nullptr) {
                break;
            }

            static std::vector<BYTE> lpb;

            UINT size;
            auto h_raw_input = (HRAWINPUT) l_param;
            UINT result = ::GetRawInputData(h_raw_input, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
            if (result) {
                throw std::runtime_error{"Failed to obtain raw input"};
            }

            lpb.reserve(size);
            result = ::GetRawInputData(h_raw_input, RID_INPUT, lpb.data(), &size, sizeof(RAWINPUTHEADER));
            if (result != size) {
                throw std::runtime_error{"Failed to obtain raw input: GetRawInputData does not return correct size"};
            }

            auto raw_input = (RAWINPUT *) lpb.data();
            switch (raw_input->header.dwType) {
                case RIM_TYPEKEYBOARD: {
                    InputDevice::RawKeyboardData data{
                        .make_code = raw_input->data.keyboard.MakeCode,
                        .flags = raw_input->data.keyboard.Flags,
                        .v_key = raw_input->data.keyboard.VKey,
                        .message = raw_input->data.keyboard.Message,
                    };
                    input_device->OnRawKeyboard(data);
                }
                case RIM_TYPEMOUSE: {
                    InputDevice::RawMouseData data{
                        .mode = raw_input->data.mouse.usFlags,
                        .button_flags = raw_input->data.mouse.usButtonFlags,
                        .extra_information = static_cast<int>(raw_input->data.mouse.ulExtraInformation),
                        .buttons = static_cast<int>(raw_input->data.mouse.ulRawButtons),
                        .wheel_delta = static_cast<short>(raw_input->data.mouse.usButtonData),
                        .x = raw_input->data.mouse.lLastX,
                        .y = raw_input->data.mouse.lLastY,
                    };
                    input_device->OnRawMouse(data);
                }
                default: {
                    break;
                }
            }

            lpb.clear();
            return DefWindowProc(hwnd, u_message, w_param, l_param);
        }
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hwnd, u_message, w_param, l_param);
        }
    }
    return 0;
}

}
