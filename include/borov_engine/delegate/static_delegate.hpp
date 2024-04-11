#pragma once

#ifndef BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED

#include <tuple>

#include "delegate_kind.hpp"

namespace borov_engine::delegate {

template <typename FunctionType, typename... Payload>
class StaticDelegate;

template <typename R, typename... Args, typename... Payload>
class StaticDelegate<R(Args...), Payload...> : public DelegateKind<R, Args...> {
  public:
    using Function = R (*)(Args..., Payload...);

    explicit StaticDelegate(Function function, Payload &&...payload);

    R Execute(Args &&...args) override;

  private:
    template <std::size_t... Is>
    R Execute_Internal(Args &&...args, std::index_sequence<Is...>);

    Function function_;
    std::tuple<Payload...> payload_;
};

}  // namespace borov_engine::delegate

#include "static_delegate.inl"

#endif  // BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED
