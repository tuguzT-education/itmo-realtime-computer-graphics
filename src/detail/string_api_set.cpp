#include "borov_engine/detail/string_api_set.hpp"

#include <stdexcept>

#include "borov_engine/detail/err_handling_api.hpp"

namespace borov_engine::detail {

std::wstring MultiByteToWideChar(UINT code_page, DWORD dw_flags, std::string_view string) {
    int result = ::MultiByteToWideChar(code_page, dw_flags, string.data(), static_cast<int>(string.size()), nullptr, 0);
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    std::wstring wstring;
    wstring.resize(result + 1, L'\0');

    result = ::MultiByteToWideChar(code_page,
                                   dw_flags,
                                   string.data(),
                                   static_cast<int>(string.size()),
                                   wstring.data(),
                                   result);
    if (result <= 0) {
        std::string message = GetLastError();
        throw std::runtime_error{message};
    }

    return wstring;
}

}
