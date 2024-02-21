#include "borov_engine/detail/err_handling_api.hpp"

#include <windows.h>

#include <stdexcept>
#include <memory>
#include <bit>

namespace borov_engine::detail {

std::string GetLastError() {
    DWORD last_error = ::GetLastError();
    if (last_error == 0) {
        return {};
    }

    LPSTR buffer = nullptr;
    DWORD dw_flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD size = FormatMessageA(dw_flags,
                                nullptr,
                                last_error,
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                std::bit_cast<LPSTR>(&buffer),
                                0,
                                nullptr);
    if (size <= 0) {
        throw std::runtime_error{"Failed to retrieve last error"};
    }

    auto deleter = [](void *ptr) { ::LocalFree(ptr); };
    std::unique_ptr<char, decltype(deleter)> unique_buffer{buffer, deleter};

    return {unique_buffer.get(), size};
}

}
