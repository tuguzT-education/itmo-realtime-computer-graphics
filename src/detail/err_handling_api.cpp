#include "borov_engine/detail/err_handling_api.hpp"

#include <windows.h>

#include <stdexcept>
#include <memory>

#include "borov_engine/detail/string_api_set.hpp"

namespace borov_engine::detail {

std::string LastError() {
    DWORD last_error = ::GetLastError();
    if (last_error == 0) {
        return {};
    }

    LPTSTR buffer = nullptr;
    DWORD dw_flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD size = FormatMessage(dw_flags,
                               nullptr,
                               last_error,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               (LPTSTR) &buffer,
                               0,
                               nullptr);
    if (size <= 0) {
        throw std::runtime_error{"Failed to retrieve last error"};
    }

    auto deleter = [](void *ptr) { ::LocalFree(ptr); };
    std::unique_ptr<TCHAR, decltype(deleter)> unique_buffer{buffer, deleter};

    return TCharToMultiByte(CP_UTF8, 0, {unique_buffer.get(), size});
}

}
