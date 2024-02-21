#include "borov_engine/detail/check_result.hpp"

#include <stdexcept>

namespace borov_engine::detail {

void CheckResult(const HRESULT result, const char *message) {
    if (FAILED(result)) {
        throw std::runtime_error{message};
    }
}

void CheckResult(const HRESULT result, const std::string &message) {
    if (FAILED(result)) {
        throw std::runtime_error{message};
    }
}

}
