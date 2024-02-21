#pragma once

#ifndef BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
#define BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED

#include <stdexcept>

namespace borov_engine::detail {

template<StringFactory F>
void CheckResult(const HRESULT result, F factory) {
    if (FAILED(result)) {
        std::string message = factory();
        throw std::runtime_error{message};
    }
}

}

#endif //BOROV_ENGINE_DETAIL_CHECK_RESULT_INL_INCLUDED
