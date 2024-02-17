#pragma once

#ifndef BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_INL_INCLUDED
#define BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_INL_INCLUDED

namespace borov_engine::delegate {

template<bool IsConst, typename T, typename R, typename... Args, typename... Payload>
SharedPtrDelegate<IsConst, T, R(Args...), Payload...>::SharedPtrDelegate(const std::shared_ptr<T> &object,
                                                                         DelegateFunction function,
                                                                         Payload &&... payload)
    : object_{object},
      function_{function},
      payload_{std::forward<Payload>(payload)...} {}

template<bool IsConst, typename T, typename R, typename... Args, typename... Payload>
R SharedPtrDelegate<IsConst, T, R(Args...), Payload...>::Execute(Args &&... args) {
    return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Payload...>());
}

template<bool IsConst, typename T, typename R, typename... Args, typename... Payload>
const void *SharedPtrDelegate<IsConst, T, R(Args...), Payload...>::GetOwner() const {
    return object_.expired() ? nullptr : object_.lock().get();
}

template<bool IsConst, typename T, typename R, typename... Args, typename... Payload>
template<std::size_t... Is>
R SharedPtrDelegate<IsConst, T, R(Args...), Payload...>::Execute_Internal(Args &&... args, std::index_sequence<Is...>) {
    if (object_.expired()) {
        return R{};
    }
    std::shared_ptr<T> pinned = object_.lock();
    return (pinned.get()->*function_)(std::forward<Args>(args)..., std::get<Is>(payload_)...);
}

}

#endif //BOROV_ENGINE_DELEGATE_SHARED_PTR_DELEGATE_INL_INCLUDED
