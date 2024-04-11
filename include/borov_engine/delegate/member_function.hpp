#pragma once

#ifndef BOROV_ENGINE_DELEGATE_MEMBER_FUNCTION_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_MEMBER_FUNCTION_HPP_INCLUDED

namespace borov_engine::delegate::detail {

template <bool IsConst, typename Object, typename RetVal, typename... Args>
struct MemberFunction;

template <typename Object, typename RetVal, typename... Args>
struct MemberFunction<true, Object, RetVal, Args...> {
    using Type = RetVal (Object::*)(Args...) const;
};

template <typename Object, typename RetVal, typename... Args>
struct MemberFunction<false, Object, RetVal, Args...> {
    using Type = RetVal (Object::*)(Args...);
};

}  // namespace borov_engine::delegate::detail

#endif  // BOROV_ENGINE_DELEGATE_MEMBER_FUNCTION_HPP_INCLUDED
