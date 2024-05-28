#pragma once

#ifndef BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
#define BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED

#include <comdef.h>

#include <stdexcept>

#include "string_api_set.hpp"

namespace borov_engine::detail {

template <StringFactory F>
void CheckResult(const HRESULT result, F &&factory) {
    if (!(FAILED(result))) {
        return;
    }

    std::string message = factory();
    const _com_error error{result};
    if (!message.empty()) {
        message.append("\ncause: ");
    }
    message.append(TCharToMultiByte(CP_UTF8, 0, error.ErrorMessage()));

    throw std::runtime_error{message};
}

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
