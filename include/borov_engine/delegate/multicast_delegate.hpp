#pragma once

#ifndef BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_HPP_INCLUDED

#include <vector>

#include "delegate.hpp"

#define DECLARE_MULTICAST_DELEGATE(name, ...) \
using name = ::borov_engine::delegate::MulticastDelegate<__VA_ARGS__>; \
using name ## Delegate = ::borov_engine::delegate::MulticastDelegate<__VA_ARGS__>::DelegateT

#define DECLARE_EVENT(name, owner_type, ...) \
class name : public ::borov_engine::delegate::MulticastDelegate<__VA_ARGS__> \
{ \
private: \
    friend class owner_type; \
    using ::borov_engine::delegate::MulticastDelegate::Broadcast; \
    using ::borov_engine::delegate::MulticastDelegate::RemoveAll; \
    using ::borov_engine::delegate::MulticastDelegate::Remove; \
};

namespace borov_engine::delegate {

class MulticastDelegateBase {
  public:
    virtual ~MulticastDelegateBase() = default;
};

// AllocatedDelegate that can be bound to by MULTIPLE objects
template<typename... Args>
class MulticastDelegate : public MulticastDelegateBase {
  public:
    using DelegateT = Delegate<void, Args...>;

  private:
    template<typename... Payload>
    using StaticKind = StaticDelegate<void(Args...), Payload...>;

    template<typename... Payload>
    using StaticFn = typename StaticKind<Payload...>::Function;

    template<typename T, typename... Payload>
    using RawKind = RawDelegate<false, T, void(Args...), Payload...>;

    template<typename T, typename... Payload>
    using RawFn = typename RawKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using ConstRawKind = RawDelegate<true, T, void(Args...), Payload...>;

    template<typename T, typename... Payload>
    using ConstRawFn = typename ConstRawKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using SharedPtrKind = SharedPtrDelegate<false, T, void(Args...), Payload...>;

    template<typename T, typename... Payload>
    using SharedPtrFn = typename SharedPtrKind<T, Payload...>::Function;

    template<typename T, typename... Payload>
    using ConstSharedPtrKind = SharedPtrDelegate<true, T, void(Args...), Payload...>;

    template<typename T, typename... Payload>
    using ConstSharedPtrFn = typename ConstSharedPtrKind<T, Payload...>::Function;

  public:
    constexpr MulticastDelegate();
    ~MulticastDelegate() noexcept override = default;

    MulticastDelegate(const MulticastDelegate &other) = default;
    MulticastDelegate &operator=(const MulticastDelegate &other) = default;

    MulticastDelegate(MulticastDelegate &&other) noexcept;
    MulticastDelegate &operator=(MulticastDelegate &&other) noexcept;

    DelegateHandle operator+=(DelegateT &&delegate);
    bool operator-=(DelegateHandle &handle);

    DelegateHandle Add(DelegateT &&delegate);

    template<typename... Payload>
    DelegateHandle AddStatic(StaticFn<Payload...> function, Payload &&... payload);

    template<typename T, typename... Payload>
    DelegateHandle AddRaw(T *object, RawFn<T, Payload...> function, Payload &&... payload);
    template<typename T, typename... Payload>
    DelegateHandle AddRaw(T *object, ConstRawFn<T, Payload...> function, Payload &&... payload);

    template<typename Lambda, typename... Payload>
    DelegateHandle AddLambda(Lambda &&lambda, Payload &&... payload);

    template<typename T, typename... Payload>
    DelegateHandle AddSharedPtr(const std::shared_ptr<T> &object,
                                SharedPtrFn<T, Payload...> function,
                                Payload &&... payload);
    template<typename T, typename... Payload>
    DelegateHandle AddSharedPtr(const std::shared_ptr<T> &object,
                                ConstSharedPtrFn<T, Payload...> function,
                                Payload &&... payload);

    [[nodiscard]] bool IsBoundTo(const DelegateHandle &handle) const;

    [[nodiscard]] constexpr std::size_t GetSize() const;

    void RemoveByOwner(void *owner);
    bool Remove(const DelegateHandle &handle);
    void RemoveAll();
    void Compress(std::size_t max_space = 0);

    void Broadcast(Args ...args);

  private:
    void Lock();
    void Unlock();

    // Returns true is the delegate is currently broadcasting
    // If this is true, the order of the array should not be changed otherwise this causes undefined behaviour
    [[nodiscard]] constexpr bool IsLocked() const;

    std::vector<DelegateT> delegates_;
    std::size_t locks_;
};

}

#include "multicast_delegate.inl"

#endif //BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_HPP_INCLUDED
