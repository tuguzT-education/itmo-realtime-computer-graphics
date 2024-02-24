#include "borov_engine/detail/check_result.hpp"

namespace borov_engine::detail {

void CheckResult(const HRESULT result, const char *message) {
    CheckResult(result, [=] { return message; });
}

void CheckResult(const HRESULT result, const std::string &message) {
    CheckResult(result, [&] { return message; });
}

}
