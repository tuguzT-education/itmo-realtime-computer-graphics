#pragma once

#ifndef BOROV_ENGINE_DELEGATE_DELEGATE_KIND_INL_INCLUDED
#define BOROV_ENGINE_DELEGATE_DELEGATE_KIND_INL_INCLUDED

namespace borov_engine::delegate {

const void *DelegateKindBase::GetOwner() const {
    return nullptr;
}

}

#endif //BOROV_ENGINE_DELEGATE_DELEGATE_KIND_INL_INCLUDED
