#pragma once

#ifndef BOROV_ENGINE_DELEGATE_LAMBDA_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_LAMBDA_DELEGATE_HPP_INCLUDED

#include <tuple>

#include "delegate_kind.hpp"

namespace borov_engine::delegate {

template<typename Lambda, typename FunctionType, typename... Payload>
class LambdaDelegate;

template<typename Lambda, typename R, typename... Args, typename... Payload>
class LambdaDelegate<Lambda, R(Args...), Payload...> : public DelegateKind<R, Args...> {
  public:
    using Function = Lambda;

    explicit LambdaDelegate(Lambda &&lambda, Payload &&... payload);

    R Execute(Args &&... args) override;

  private:
    template<std::size_t... Is>
    R Execute_Internal(Args &&... args, std::index_sequence<Is...>);

    Lambda lambda_;
    std::tuple<Payload...> payload_;
};

}

#include "lambda_delegate.inl"

#endif //BOROV_ENGINE_DELEGATE_LAMBDA_DELEGATE_HPP_INCLUDED
