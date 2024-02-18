#pragma once

#ifndef BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_INL_INCLUDED
#define BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_INL_INCLUDED

namespace borov_engine::delegate {

constexpr DelegateHandle::DelegateHandle(std::nullptr_t) noexcept: id_{kInvalidID} {}

constexpr DelegateHandle::DelegateHandle(DelegateHandle &&other) noexcept: id_{other.id_} {
    other.Reset();
}

constexpr DelegateHandle &DelegateHandle::operator=(DelegateHandle &&other) noexcept {
    id_ = other.id_;
    other.Reset();
    return *this;
}

constexpr DelegateHandle::operator bool() const noexcept {
    return IsValid();
}

constexpr bool DelegateHandle::IsValid() const noexcept {
    return id_ != kInvalidID;
}

constexpr void DelegateHandle::Reset() noexcept {
    id_ = kInvalidID;
}

}

#endif //BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_INL_INCLUDED
