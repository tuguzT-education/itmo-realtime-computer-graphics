#pragma once

#ifndef BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_INL_INCLUDED
#define BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_INL_INCLUDED

namespace borov_engine::delegate {

template<typename... Args>
constexpr MulticastDelegate<Args...>::MulticastDelegate() : locks_{} {}

template<typename... Args>
MulticastDelegate<Args...>::MulticastDelegate(MulticastDelegate &&other) noexcept
    : delegates_{std::move(other.delegates_)},
      locks_{std::move(other.locks_)} {}

template<typename... Args>
MulticastDelegate<Args...> &MulticastDelegate<Args...>::operator=(MulticastDelegate &&other) noexcept {
    delegates_ = std::move(other.delegates_);
    locks_ = std::move(other.locks_);
    return *this;
}

template<typename... Args>
DelegateHandle MulticastDelegate<Args...>::operator+=(MulticastDelegate::DelegateT &&delegate) {
    return Add(std::forward<DelegateT>(delegate));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
template<typename... Args>
bool MulticastDelegate<Args...>::operator-=(DelegateHandle &handle) {
    return Remove(handle);
}
#pragma clang diagnostic pop

template<typename... Args>
DelegateHandle MulticastDelegate<Args...>::Add(MulticastDelegate::DelegateT &&delegate) {
    // Favour an empty space over a possible array reallocation
    for (DelegateT &item : delegates_) {
        if (item.IsBound()) {
            continue;
        }
        item = std::move(delegate);
        return item.Handle();
    }
    DelegateT &item = delegates_.emplace_back(std::move(delegate));
    return item.Handle();
}

template<typename... Args>
template<typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddStatic(MulticastDelegate::StaticFn<Payload...> function,
                                                     Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateStatic(function, std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
template<typename T, typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddRaw(T *object,
                                                  MulticastDelegate::RawFn<T, Payload...> function,
                                                  Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateRaw(object, function, std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
template<typename T, typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddRaw(T *object,
                                                  MulticastDelegate::ConstRawFn<T, Payload...> function,
                                                  Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateRaw(object, function, std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
template<typename Lambda, typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddLambda(Lambda &&lambda, Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateLambda(std::forward<Lambda>(lambda), std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
template<typename T, typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddSharedPtr(const std::shared_ptr<T> &object,
                                                        MulticastDelegate::SharedPtrFn<T, Payload...> function,
                                                        Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateSharedPtr(object, function, std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
template<typename T, typename... Payload>
DelegateHandle MulticastDelegate<Args...>::AddSharedPtr(const std::shared_ptr<T> &object,
                                                        MulticastDelegate::ConstSharedPtrFn<T, Payload...> function,
                                                        Payload &&... payload) {
    DelegateT delegate = DelegateT::CreateSharedPtr(object, function, std::forward<Payload>(payload)...);
    return Add(std::move(delegate));
}

template<typename... Args>
bool MulticastDelegate<Args...>::IsBoundTo(const DelegateHandle &handle) const {
    if (!handle.IsValid()) {
        return false;
    }

    auto pred = [handle](const DelegateT &delegate) {
        return delegate.Handle() == handle;
    };
    auto iter = std::find_if(delegates_.begin(), delegates_.end(), pred);
    return iter != delegates_.end();
}

template<typename... Args>
constexpr std::size_t MulticastDelegate<Args...>::GetSize() const {
    return delegates_.size();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
template<typename... Args>
void MulticastDelegate<Args...>::RemoveByOwner(void *owner) {
    if (owner == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < delegates_.size(); ++i) {
        DelegateT &delegate = delegates_[i];
        if (delegate.Owner() != owner) {
            continue;
        }

        if (IsLocked()) {
            delegate.Clear();
        } else {
            std::swap(delegate, delegates_.back());
            delegates_.pop_back();
        }
    }
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
#pragma ide diagnostic ignored "ConstantFunctionResult"
template<typename... Args>
bool MulticastDelegate<Args...>::Remove(const DelegateHandle &handle) {
    if (!handle.IsValid()) {
        return false;
    }

    for (std::size_t i = 0; i < delegates_.size(); ++i) {
        DelegateT &delegate = delegates_[i];
        if (delegate.Handle() != handle) {
            continue;
        }

        if (IsLocked()) {
            delegate.Clear();
        } else {
            std::swap(delegate, delegates_.back());
            delegates_.pop_back();
        }
        return true;
    }
    return false;
}
#pragma clang diagnostic pop

template<typename... Args>
void MulticastDelegate<Args...>::RemoveAll() {
    if (IsLocked()) {
        for (DelegateT &delegate : delegates_) {
            delegate.Clear();
        }
    } else {
        delegates_.clear();
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
template<typename... Args>
void MulticastDelegate<Args...>::Compress(std::size_t max_space) {
    if (IsLocked()) {
        return;
    }

    std::size_t to_delete = 0;
    for (std::size_t i = 0; i < delegates_.size() - to_delete; ++i) {
        DelegateT &delegate = delegates_[i];
        if (delegate.Handle().IsValid()) {
            continue;
        }
        std::swap(delegate, delegates_[to_delete]);
        ++to_delete;
    }

    if (to_delete > max_space) {
        delegates_.resize(delegates_.size() - to_delete);
    }
}
#pragma clang diagnostic pop

template<typename... Args>
void MulticastDelegate<Args...>::Broadcast(Args... args) {
    Lock();
    for (DelegateT &delegate : delegates_) {
        delegate.ExecuteIfBound(std::forward<Args>(args)...);
    }
    Unlock();
}

template<typename... Args>
void MulticastDelegate<Args...>::Lock() {
    ++locks_;
}

template<typename... Args>
void MulticastDelegate<Args...>::Unlock() {
    // Unlock() should never be called more than Lock()!
    assert(locks_ > 0);
    --locks_;
}

template<typename... Args>
constexpr bool MulticastDelegate<Args...>::IsLocked() const {
    return locks_ > 0;
}

}

#endif //BOROV_ENGINE_DELEGATE_MULTICAST_DELEGATE_INL_INCLUDED
