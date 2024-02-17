#pragma once

#ifndef BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED

#include <tuple>
#include <memory>

#include "delegate_kind.hpp"
#include "member_function.hpp"

namespace borov_engine::delegate {

template<bool IsConst, typename T, typename FunctionType, typename... Payload>
class SharedPtrDelegate;

template<bool IsConst, typename T, typename R, typename... Args, typename... Payload>
class SharedPtrDelegate<IsConst, T, R(Args...), Payload...> : public DelegateKind<R, Args...> {
    using DelegateFunction = typename detail::MemberFunction<IsConst, T, R, Args..., Payload...>::Type;

  public:
    SharedPtrDelegate(const std::shared_ptr<T> &object, DelegateFunction function, Payload &&... payload);

    R Execute(Args &&... args) override;

    [[nodiscard]] const void *GetOwner() const override;

  private:
    template<std::size_t... Is>
    R Execute_Internal(Args &&... args, std::index_sequence<Is...>);

    std::weak_ptr<T> object_;
    DelegateFunction function_;
    std::tuple<Payload...> payload_;
};

}

#include "shared_ptr_delegate.inl"

#endif //BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_HPP_INCLUDED
