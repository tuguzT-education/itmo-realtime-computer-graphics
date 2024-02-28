/**
DOCUMENTATION

## Classes ##
- ```AllocatedDelegate<RetVal, Args>```
- ```MulticastDelegate<Args>```

## Features ##
- Support for:
	- Static/Global methods
	- Member functions
	- Lambda's
	- std::shared_ptr
- AllocatedDelegate object is allocated inline if it is under 32 bytes
- Add payload to delegate during bind-time
- Move operations enable optimization

## Example Usage ##

### AllocatedDelegate ###

AllocatedDelegate<int, float> del;
del.BindLambda([](float a, int payload)
{
	std::cout << "Lambda delegate parameter: " << a << std::endl;
	std::cout << "Lambda delegate payload: " << payload << std::endl;
	return 10;
}, 50);
std::cout << "Lambda delegate return value: " << del.Execute(20) << std::endl;

Output:
Lambda delegate parameter: 20
Lambda delegate payload: 50
Lambda delegate return value: 10

### MulticastDelegate ###

struct Foo
{
	void Bar(float a, int payload)
	{
		std::cout << "Raw delegate parameter: " << a << std::endl;
		std::cout << "Raw delegate payload: " << payload << std::endl;
	}
};
MulticastDelegate<float> del;
del.AddLambda([](float a, int payload)
{
	std::cout << "Lambda delegate parameter: " << a << std::endl;
	std::cout << "Lambda delegate payload: " << payload << std::endl;
}, 90);

Foo foo;
del.AddRaw(&foo, &Foo::Bar, 10);
del.Broadcast(20);

Output:
Lambda delegate parameter: 20
Lambda delegate payload: 90
Raw delegate parameter: 20
Raw delegate payload: 10

*/

#pragma once

#ifndef BOROV_ENGINE_DELEGATE_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_DELEGATE_HPP_INCLUDED

#include <optional>
#include <type_traits>
#include <cassert>

#include "../alloc/inline_allocator.hpp"
#include "member_function.hpp"
#include "delegate_kind.hpp"
#include "static_delegate.hpp"
#include "raw_delegate.hpp"
#include "lambda_delegate.hpp"
#include "shared_ptr_delegate.hpp"
#include "delegate_handle.hpp"

#define DECLARE_DELEGATE(name, ...) \
using name = ::borov_engine::delegate::Delegate<void, __VA_ARGS__>

#define DECLARE_DELEGATE_RETURN(name, return_type, ...) \
using name = ::borov_engine::delegate::Delegate<return_type, __VA_ARGS__>

namespace borov_engine::delegate {

// The allocation size of delegate data.
// Delegates larger than this will be heap allocated.
constexpr std::size_t kDelegateInlineAllocationSize = 32;

class DelegateBase {
  public:
    explicit DelegateBase() noexcept;
    constexpr explicit DelegateBase(std::nullptr_t) noexcept;
    virtual ~DelegateBase();

    DelegateBase(const DelegateBase &other) = default;
    DelegateBase &operator=(const DelegateBase &other);

    DelegateBase(DelegateBase &&other) noexcept;
    DelegateBase &operator=(DelegateBase &&other) noexcept;

    [[nodiscard]] const void *Owner() const;

    [[nodiscard]] constexpr DelegateHandle Handle() const;

    void ClearIfBoundTo(void *object);

    void Clear();

    [[nodiscard]] constexpr bool IsBound() const;

    bool IsBoundTo(void *object) const;

  protected:
    void Release();

    [[nodiscard]] constexpr DelegateKindBase *AllocatedDelegate() const;

    DelegateHandle handle_;
    alloc::InlineAllocator<kDelegateInlineAllocationSize> allocator_;
};

constexpr DelegateBase::DelegateBase(std::nullptr_t) noexcept: handle_{nullptr}, allocator_{} {}

constexpr DelegateHandle DelegateBase::Handle() const {
    return handle_;
}

constexpr bool DelegateBase::IsBound() const {
    return allocator_.HasAllocation();
}

constexpr DelegateKindBase *DelegateBase::AllocatedDelegate() const {
    return static_cast<DelegateKindBase *>(allocator_.Allocation());
}

// AllocatedDelegate that can be bound to by just ONE object
template<typename R, typename... Args>
class Delegate : public DelegateBase {
  private:
    template<typename... Payload>
    using StaticKind = StaticDelegate<R(Args...), Payload...>;

    template<typename... Payload>
    using StaticFn = typename StaticKind<Payload...>::Function;

    template<typename T, typename... Payload>
    using RawKind = RawDelegate<false, T, R(Args...), Payload...>;

    template<typename T, typename... Payload>
    using RawFn = typename RawKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using ConstRawKind = RawDelegate<true, T, R(Args...), Payload...>;

    template<typename T, typename... Payload>
    using ConstRawFn = typename ConstRawKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using SharedPtrKind = SharedPtrDelegate<false, T, R(Args...), Payload...>;

    template<typename T, typename... Payload>
    using SharedPtrFn = typename SharedPtrKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using ConstSharedPtrKind = SharedPtrDelegate<true, T, R(Args...), Payload...>;

    template<typename T, typename... Payload>
    using ConstSharedPtrFn = typename ConstSharedPtrKind<T, Payload...>::Function;

  public:
    using IDelegateT = DelegateKind<R, Args...>;

    template<typename... Payload>
    [[nodiscard]] static Delegate CreateStatic(StaticFn<Payload...> function, Payload... payload);

    template<typename T, typename... Payload>
    [[nodiscard]] static Delegate CreateRaw(T *object, RawFn<T, Payload...> function, Payload... payload);
    template<typename T, typename... Payload>
    [[nodiscard]] static Delegate CreateRaw(T *object, ConstRawFn<T, Payload...> function, Payload... payload);

    template<typename T, typename... Payload>
    [[nodiscard]] static Delegate CreateSharedPtr(const std::shared_ptr<T> &object,
                                                  SharedPtrFn<T, Payload...> function,
                                                  Payload... payload);
    template<typename T, typename... Payload>
    [[nodiscard]] static Delegate CreateSharedPtr(const std::shared_ptr<T> &object,
                                                  ConstSharedPtrFn<T, Payload...> function,
                                                  Payload... payload);

    template<typename Lambda, typename... Payload>
    [[nodiscard]] static Delegate CreateLambda(Lambda &&lambda, Payload... payload);

    template<typename T, typename... Payload>
    void BindRaw(T *object, RawFn<T, Payload...> function, Payload &&... payload);
    template<typename T, typename... Payload>
    void BindRaw(T *object, ConstRawFn<T, Payload...> function, Payload &&... payload);

    template<typename... Payload>
    void BindStatic(StaticFn<Payload...> function, Payload &&... payload);

    template<typename Lambda, typename... Payload>
    void BindLambda(Lambda &&lambda, Payload &&... payload);

    template<typename T, typename... Payload>
    void BindSharedPtr(std::shared_ptr<T> object, SharedPtrFn<T, Payload...> function, Payload &&... payload);
    template<typename T, typename... Payload>
    void BindSharedPtr(std::shared_ptr<T> object, ConstSharedPtrFn<T, Payload...> function, Payload &&... payload);

    R Execute(Args... args) const;

    std::optional<R> ExecuteIfBound(Args... args) const;
    R ExecuteIfBound(Args... args) const requires(std::is_void_v<R>);

  private:
    template<typename T, typename... TArgs>
    void Bind(TArgs &&... args);
};

template<typename R, typename... Args>
template<typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateStatic(StaticFn<Payload...> function, Payload... payload) {
    Delegate handler;
    handler.Bind<StaticKind<Payload...>>(function, std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateRaw(T *object, RawFn<T, Payload...> function, Payload... payload) {
    Delegate handler;
    handler.Bind<RawKind<T, Payload...>>(object, function, std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateRaw(T *object,
                                                     ConstRawFn<T, Payload...> function,
                                                     Payload... payload) {
    Delegate handler;
    handler.Bind<ConstRawKind<T, Payload...>>(object, function, std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateSharedPtr(const std::shared_ptr<T> &object,
                                                           SharedPtrFn<T, Payload...> function,
                                                           Payload... payload) {
    Delegate handler;
    handler.Bind<SharedPtrKind<T, Payload...>>(object, function, std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateSharedPtr(const std::shared_ptr<T> &object,
                                                           ConstSharedPtrFn<T, Payload...> function,
                                                           Payload... payload) {
    Delegate handler;
    handler.Bind<ConstSharedPtrKind<T, Payload...>>(object, function, std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename Lambda, typename... Payload>
Delegate<R, Args...> Delegate<R, Args...>::CreateLambda(Lambda &&lambda, Payload... payload) {
    Delegate handler;
    handler.Bind<LambdaDelegate<Lambda, R(Args...), Payload...>>(std::forward<Lambda>(lambda),
                                                                 std::forward<Payload>(payload)...);
    return handler;
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
void Delegate<R, Args...>::BindRaw(T *object, RawFn<T, Payload...> function, Payload &&... payload) {
    static_assert(!std::is_const_v<T>, "Cannot bind a non-const function on a const object");
    *this = CreateRaw<T, Payload...>(object, function, std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
void Delegate<R, Args...>::BindRaw(T *object, ConstRawFn<T, Payload...> function, Payload &&... payload) {
    *this = CreateRaw<T, Payload...>(object, function, std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
template<typename... Payload>
void Delegate<R, Args...>::BindStatic(StaticFn<Payload...> function, Payload &&... payload) {
    *this = CreateStatic<Payload...>(function, std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
template<typename Lambda, typename... Payload>
void Delegate<R, Args...>::BindLambda(Lambda &&lambda, Payload &&... payload) {
    *this = CreateLambda<Lambda, Payload...>(std::forward<Lambda>(lambda), std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
void Delegate<R, Args...>::BindSharedPtr(std::shared_ptr<T> object,
                                         SharedPtrFn<T, Payload...> function,
                                         Payload &&... payload) {
    static_assert(!std::is_const_v<T>, "Cannot bind a non-const function on a const object");
    *this = CreateSharedPtr<T, Payload...>(object, function, std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
template<typename T, typename... Payload>
void Delegate<R, Args...>::BindSharedPtr(std::shared_ptr<T> object,
                                         ConstSharedPtrFn<T, Payload...> function,
                                         Payload &&... payload) {
    *this = CreateSharedPtr<T, Payload...>(object, function, std::forward<Payload>(payload)...);
}

template<typename R, typename... Args>
R Delegate<R, Args...>::Execute(Args... args) const {
    assert(allocator_.HasAllocation() && "AllocatedDelegate is not bound");

    auto delegate = dynamic_cast<IDelegateT *>(AllocatedDelegate());
    return delegate->Execute(std::forward<Args>(args)...);
}

template<typename R, typename... Args>
std::optional<R> Delegate<R, Args...>::ExecuteIfBound(Args... args) const {
    if (!IsBound()) {
        return std::nullopt;
    }
    auto delegate = dynamic_cast<IDelegateT *>(AllocatedDelegate());
    return delegate->Execute(std::forward<Args>(args)...);
}

template<typename R, typename... Args>
R Delegate<R, Args...>::ExecuteIfBound(Args... args) const requires(std::is_void_v<R>)  {
    if (!IsBound()) {
        return;
    }
    auto delegate = dynamic_cast<IDelegateT *>(AllocatedDelegate());
    return delegate->Execute(std::forward<Args>(args)...);
}

template<typename R, typename... Args>
template<typename T, typename... TArgs>
void Delegate<R, Args...>::Bind(TArgs &&... args) {
    Release();
    T *alloc = static_cast<T *>(allocator_.Allocate(sizeof(T)));
    std::construct_at(alloc, std::forward<TArgs>(args)...);
}

}

#endif //BOROV_ENGINE_DELEGATE_DELEGATE_HPP_INCLUDED
