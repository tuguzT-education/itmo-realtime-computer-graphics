#include "borov_engine/detail/string_api_set.hpp"

#include <stdexcept>

#include "borov_engine/detail/err_handling_api.hpp"

namespace borov_engine::detail {

std::wstring MultiByteToWideChar(UINT code_page, DWORD dw_flags, std::string_view view) {
    int result = ::MultiByteToWideChar(code_page, dw_flags, view.data(), static_cast<int>(view.size()), nullptr, 0);
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    std::wstring string(result, L'\0');
    result = ::MultiByteToWideChar(code_page,
                                   dw_flags,
                                   view.data(),
                                   static_cast<int>(view.size()),
                                   string.data(),
                                   result);
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    return string;
}

std::string WideCharToMultiByte(UINT code_page, DWORD dw_flags, std::wstring_view view,
                                const char *default_char, bool *used_default_char) {
    int result = ::WideCharToMultiByte(code_page,
                                       dw_flags,
                                       view.data(),
                                       static_cast<int>(view.size()),
                                       nullptr,
                                       0,
                                       default_char,
                                       reinterpret_cast<LPBOOL>(used_default_char));
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    std::string string(result, '\0');
    result = ::WideCharToMultiByte(code_page,
                                   dw_flags,
                                   view.data(),
                                   static_cast<int>(view.size()),
                                   string.data(),
                                   result,
                                   default_char,
                                   reinterpret_cast<LPBOOL>(used_default_char));
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    return string;
}

std::basic_string<TCHAR> MultiByteToTChar([[maybe_unused]] UINT code_page,
                                          [[maybe_unused]] DWORD dw_flags,
                                          std::string_view view) {
#ifdef UNICODE
    return MultiByteToWideChar(code_page, dw_flags, view);
#else
    return {view.data(), view.size()};
#endif
}

std::string TCharToMultiByte([[maybe_unused]] UINT code_page,
                             [[maybe_unused]] DWORD dw_flags,
                             std::basic_string_view<TCHAR> view,
                             [[maybe_unused]] const char *default_char,
                             [[maybe_unused]] bool *used_default_char) {
#ifdef UNICODE
    return WideCharToMultiByte(code_page, dw_flags, view, default_char, used_default_char);
#else
    return {view.data(), view.size()};
#endif
}

}
