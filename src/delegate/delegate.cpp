#include "borov_engine/delegate/delegate.hpp"

namespace borov_engine::delegate {

DelegateBase::DelegateBase() noexcept = default;

DelegateBase::~DelegateBase() {
    Release();
}

DelegateBase &DelegateBase::operator=(const DelegateBase &other) {
    Release();
    handle_ = other.handle_;
    allocator_ = other.allocator_;
    return *this;
}

DelegateBase::DelegateBase(DelegateBase &&other) noexcept
    : handle_{std::move(other.handle_)}, allocator_{std::move(other.allocator_)} {}

DelegateBase &DelegateBase::operator=(DelegateBase &&other) noexcept {
    Release();
    handle_ = std::move(other.handle_);
    allocator_ = std::move(other.allocator_);
    return *this;
}

const void *DelegateBase::Owner() const {
    if (!IsBound()) {
        return nullptr;
    }
    return AllocatedDelegate()->Owner();
}

void DelegateBase::ClearIfBoundTo(void *object) {
    if (IsBoundTo(object)) {
        Release();
    }
}

void DelegateBase::Clear() {
    Release();
}

bool DelegateBase::IsBoundTo(void *object) const {
    if (object == nullptr || !IsBound()) {
        return false;
    }
    return AllocatedDelegate()->Owner() == object;
}

void DelegateBase::Release() {
    if (!IsBound()) {
        return;
    }
    handle_.Reset();
    std::destroy_at(AllocatedDelegate());
    allocator_.Free();
}

}  // namespace borov_engine::delegate
