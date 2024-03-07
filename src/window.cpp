#include "borov_engine/window.hpp"

#include <stdexcept>
#include <vector>

#include "borov_engine/detail/string_api_set.hpp"
#include "borov_engine/input.hpp"

namespace borov_engine {

Window::Window(std::string_view name,
               std::int32_t width, std::int32_t height,
               std::int32_t min_width, std::int32_t min_height,
               HINSTANCE instance_handle) : input_{}, is_destroyed_{}, min_width_{min_width}, min_height_{min_height} {
    instance_handle = (instance_handle != nullptr)
                      ? instance_handle
                      : GetModuleHandle(nullptr);

    std::basic_string<TCHAR> t_name = detail::MultiByteToTChar(CP_UTF8, 0, name);
    LPCTSTR c_name = t_name.c_str();
    WNDCLASSEX wc{
        .cbSize = sizeof(decltype(wc)),
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = WndProc,
        .hInstance = instance_handle,
        .hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
        .lpszClassName = c_name,
    };
    RegisterClassEx(&wc);

    RECT rect{
        .right = width,
        .bottom = height,
    };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    handle_ = CreateWindowEx(WS_EX_APPWINDOW,
                             c_name,
                             c_name,
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

Window::~Window() {
    Destroy();
}

HWND Window::RawHandle() const {
    return handle_;
}

HINSTANCE Window::RawInstanceHandle() const {
    return (HINSTANCE) GetWindowLongPtr(handle_, GWLP_HINSTANCE);
}

math::Rectangle Window::Dimensions() const {
    RECT rect{};
    GetWindowRect(handle_, &rect);
    return math::Rectangle{rect};
}

math::Rectangle Window::ClientDimensions() const {
    RECT rect{};
    ::GetClientRect(handle_, &rect);
    return math::Rectangle{rect};
}

std::int32_t Window::MinWidth() const {
    return min_width_;
}

std::int32_t &Window::MinWidth() {
    return min_width_;
}

std::int32_t Window::MinHeight() const {
    return min_height_;
}

std::int32_t &Window::MinHeight() {
    return min_height_;
}

bool Window::IsDestroyed() const {
    return is_destroyed_;
}

bool Window::IsFocused() const {
    return handle_ == GetFocus();
}

std::string Window::Title() const {
    auto size = GetWindowTextLength(handle_);
    std::basic_string<TCHAR> title(size, TEXT('\0'));
    GetWindowText(handle_, title.data(), size + 1);
    return detail::TCharToMultiByte(CP_UTF8, 0, title);
}

bool Window::Title(std::string_view title) {
    std::basic_string<TCHAR> t_title = detail::MultiByteToTChar(CP_UTF8, 0, title);
    LPCTSTR c_title = t_title.c_str();
    return SetWindowText(handle_, c_title);
}

const OnWindowResize &Window::OnResize() const {
    return on_resize_;
}

OnWindowResize &Window::OnResize() {
    return on_resize_;
}

void Window::ProcessQueueMessages() {
    MSG msg;
    while (PeekMessage(&msg, handle_, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::Destroy() {
    if (IsDestroyed()) {
        return;
    }
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
            Input *input = window->input_;
            if (input == nullptr) {
                break;
            }
            if (!window->IsFocused()) {
                break;
            }

            static std::vector<BYTE> raw_input_bytes;

            UINT size;
            auto h_raw_input = (HRAWINPUT) l_param;
            UINT result = ::GetRawInputData(h_raw_input, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
            if (result) {
                throw std::runtime_error{"Failed to obtain raw input"};
            }

            raw_input_bytes.reserve(size);
            result = ::GetRawInputData(h_raw_input, RID_INPUT, raw_input_bytes.data(), &size, sizeof(RAWINPUTHEADER));
            if (result != size) {
                throw std::runtime_error{"Failed to obtain raw input: GetRawInputData does not return correct size"};
            }

            auto raw_input = (RAWINPUT *) raw_input_bytes.data();
            switch (raw_input->header.dwType) {
                case RIM_TYPEKEYBOARD: {
                    input->OnRawKeyboard(raw_input->data.keyboard);
                    break;
                }
                case RIM_TYPEMOUSE: {
                    input->OnRawMouse(raw_input->data.mouse);
                    break;
                }
                default: {
                    break;
                }
            }

            raw_input_bytes.clear();
            break;
        }
        case WM_GETMINMAXINFO: {
            auto min_max_info = (LPMINMAXINFO) l_param;
            min_max_info->ptMinTrackSize = {
                .x = window->min_width_,
                .y = window->min_height_,
            };
            break;
        }
        case WM_SIZE: {
            LONG width = LOWORD(l_param);
            LONG height = HIWORD(l_param);
            window->on_resize_.Broadcast(WindowResizeData{width, height});
            break;
        }
        case WM_CLOSE: {
            window->Destroy();
            break;
        }
        case WM_DESTROY: {
            window->is_destroyed_ = true;
            break;
        }
        default: {
            return DefWindowProc(hwnd, u_message, w_param, l_param);
        }
    }
    return 0;
}

}
