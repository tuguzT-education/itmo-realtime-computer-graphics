#pragma once

#ifndef BOROV_ENGINE_DELEGATE_RAW_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_RAW_DELEGATE_HPP_INCLUDED

#include <tuple>

#include "delegate_kind.hpp"
#include "member_function.hpp"

namespace borov_engine::delegate {

template <bool IsConst, typename T, typename FunctionType, typename... Payload>
class RawDelegate;

template <bool IsConst, typename T, typename R, typename... Args, typename... Payload>
class RawDelegate<IsConst, T, R(Args...), Payload...> : public DelegateKind<R, Args...> {
  public:
    using Function = typename detail::MemberFunction<IsConst, T, R, Args..., Payload...>::Type;

    RawDelegate(T *object, Function function, Payload &&...payload);

    R Execute(Args &&...args) override;

    [[nodiscard]] const void *Owner() const override;

  private:
    template <std::size_t... Is>
    R Execute_Internal(Args &&...args, std::index_sequence<Is...>);

    T *object_;
    Function function_;
    std::tuple<Payload...> payload_;
};

}  // namespace borov_engine::delegate

#include "raw_delegate.inl"

#endif  // BOROV_ENGINE_DELEGATE_RAW_DELEGATE_HPP_INCLUDED
