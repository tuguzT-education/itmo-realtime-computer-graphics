#pragma once

#ifndef BOROV_ENGINE_DELEGATE_DELEGATE_KIND_HPP_INCLUDED
#define BOROV_ENGINE_DELEGATE_DELEGATE_KIND_HPP_INCLUDED

namespace borov_engine::delegate {

class DelegateKindBase {
  public:
    DelegateKindBase();
    virtual ~DelegateKindBase() noexcept;

    [[nodiscard]] virtual const void *Owner() const;
};

template <typename R, typename... Args>
class DelegateKind : public DelegateKindBase {
  public:
    [[nodiscard]] virtual R Execute(Args &&...args) = 0;
};

}  // namespace borov_engine::delegate

#include "delegate_kind.inl"

#endif  // BOROV_ENGINE_DELEGATE_DELEGATE_KIND_HPP_INCLUDED
