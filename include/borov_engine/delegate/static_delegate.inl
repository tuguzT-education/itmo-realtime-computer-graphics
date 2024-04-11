#pragma once

#ifndef BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_INL_INCLUDED
#define BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_INL_INCLUDED

namespace borov_engine::delegate {

template <typename R, typename... Args, typename... Payload>
StaticDelegate<R(Args...), Payload...>::StaticDelegate(Function function, Payload &&...payload)
    : function_{function}, payload_{std::forward<Payload>(payload)...} {}

template <typename R, typename... Args, typename... Payload>
R StaticDelegate<R(Args...), Payload...>::Execute(Args &&...args) {
    return Execute_Internal(std::forward<Args>(args)..., std::index_sequence_for<Payload...>());
}

template <typename R, typename... Args, typename... Payload>
template <std::size_t... Is>
R StaticDelegate<R(Args...), Payload...>::Execute_Internal(Args &&...args, std::index_sequence<Is...>) {
    return function_(std::forward<Args>(args)..., std::get<Is>(payload_)...);
}

}  // namespace borov_engine::delegate

#endif  // BOROV_ENGINE_DELEGATE_STATIC_DELEGATE_INL_INCLUDED
