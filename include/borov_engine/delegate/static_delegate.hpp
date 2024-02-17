#pragma once

#ifndef BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED

#include <tuple>

#include "delegate_kind.hpp"

namespace borov_engine::delegate {

template<typename FunctionType, typename... Payload>
class StaticDelegate;

template<typename R, typename... Args, typename... Payload>
class StaticDelegate<R(Args...), Payload...> : public DelegateKind<R, Args...> {
    using DelegateFunction = R(*)(Args..., Payload...);

  public:
    explicit StaticDelegate(DelegateFunction function, Payload &&... payload);

    R Execute(Args &&... args) override;

  private:
    template<std::size_t... Is>
    R Execute_Internal(Args &&... args, std::index_sequence<Is...>);

    DelegateFunction function_;
    std::tuple<Payload...> payload_;
};

}

#include "static_delegate.inl"

#endif //BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_HPP_INCLUDED
