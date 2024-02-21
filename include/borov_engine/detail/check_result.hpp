#pragma once

#ifndef BOROV_ENGINE_CHECK_RESULT_HPP_INCLUDED
#define BOROV_ENGINE_CHECK_RESULT_HPP_INCLUDED

#include <windows.h>

#include <string>

namespace borov_engine::detail {

void CheckResult(HRESULT result, const char *message = nullptr);
void CheckResult(HRESULT result, const std::string &message);

template<typename F>
concept StringFactory = requires(F factory) {
    { factory() } -> std::convertible_to<std::string>;
};

template<StringFactory F>
void CheckResult(HRESULT result, F factory);

}

#include "check_result.inl"

#endif //BOROV_ENGINE_CHECK_RESULT_HPP_INCLUDED
