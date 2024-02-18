#pragma once

#ifndef BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_HPP_INCLUDED

#include <compare>

namespace borov_engine::delegate {

class DelegateHandle {
  public:
    using ID = std::size_t;

    explicit DelegateHandle() noexcept;
    constexpr explicit DelegateHandle(std::nullptr_t) noexcept;

    constexpr ~DelegateHandle() noexcept = default;

    constexpr DelegateHandle(const DelegateHandle &other) noexcept = default;
    constexpr DelegateHandle &operator=(const DelegateHandle &other) noexcept = default;

    constexpr DelegateHandle(DelegateHandle &&other) noexcept;
    constexpr DelegateHandle &operator=(DelegateHandle &&other) noexcept;

    constexpr auto operator<=>(const DelegateHandle &other) const noexcept = default;
    explicit constexpr operator bool() const noexcept;

    [[nodiscard]] constexpr bool IsValid() const noexcept;

    constexpr void Reset() noexcept;

  private:
    static const ID kInvalidID;
    static ID GenerateNewID() noexcept;

    ID id_;
};

}

#include "delegate_handle.inl"

#endif //BOROV_ENGINE_DELEGATE_DELEGATE_HANDLE_HPP_INCLUDED
