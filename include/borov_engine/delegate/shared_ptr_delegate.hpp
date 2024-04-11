#pragma once

#ifndef BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED

#include <memory>
#include <tuple>

#include "delegate_kind.hpp"
#include "member_function.hpp"

namespace borov_engine::delegate {

template <bool IsConst, typename T, typename FunctionType, typename... Payload>
class SharedPtrDelegate;

template <bool IsConst, typename T, typename R, typename... Args, typename... Payload>
class SharedPtrDelegate<IsConst, T, R(Args...), Payload...> : public DelegateKind<R, Args...> {
  public:
    using Function = typename detail::MemberFunction<IsConst, T, R, Args..., Payload...>::Type;

    SharedPtrDelegate(const std::shared_ptr<T> &object, Function function, Payload &&...payload);

    R Execute(Args &&...args) override;

    [[nodiscard]] const void *Owner() const override;

  private:
    template <std::size_t... Is>
    R Execute_Internal(Args &&...args, std::index_sequence<Is...>);

    std::weak_ptr<T> object_;
    Function function_;
    std::tuple<Payload...> payload_;
};

}  // namespace borov_engine::delegate

#include "shared_ptr_delegate.inl"

#endif  // BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED
