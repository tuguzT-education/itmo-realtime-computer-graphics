#include "borov_engine/delegate/delegate_kind.hpp"

namespace borov_engine::delegate {

DelegateKindBase::DelegateKindBase() = default;

DelegateKindBase::~DelegateKindBase() noexcept = default;

const void *DelegateKindBase::Owner() const {
    return nullptr;
}

}  // namespace borov_engine::delegate
