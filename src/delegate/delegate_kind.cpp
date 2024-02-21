#include "borov_engine/delegate/delegate_kind.hpp"

namespace borov_engine::delegate {

const void *DelegateKindBase::GetOwner() const {
    return nullptr;
}

}
