#pragma once

#ifndef BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
#define BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED

#include <stdexcept>

#include <comdef.h>

#include "string_api_set.hpp"

namespace borov_engine::detail {

template<StringFactory F>
void CheckResult(const HRESULT result, F &&factory) {
    if (FAILED(result)) {
        std::string message = factory();

        _com_error error{result};
        if (!message.empty()) {
            message.append("\ncause: ");
        }
        message.append(TCharToMultiByte(CP_UTF8, 0, error.ErrorMessage()));

        throw std::runtime_error{message};
    }
}

}

#endif //BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
