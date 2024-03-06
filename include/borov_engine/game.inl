#pragma once

#ifndef BOROV_ENGINE_GAME_INL_INCLUDED
#define BOROV_ENGINE_GAME_INL_INCLUDED

namespace borov_engine {

template<typename T, typename... Args>
T &Game::AddComponent(Args &&... args) {
    auto &component = components_.emplace_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
    return dynamic_cast<T &>(*component);
}

ConstComponentView auto Game::Components() const {
    auto unique_ptr_to_ref = [](const std::unique_ptr<Component> &component) {
        return std::cref(*component);
    };
    return components_ | std::ranges::views::transform(unique_ptr_to_ref);
}

ComponentView auto Game::Components() {
    auto unique_ptr_to_ref = [](std::unique_ptr<Component> &component) {
        return std::ref(*component);
    };
    return components_ | std::ranges::views::transform(unique_ptr_to_ref);
}

}

#endif //BOROV_ENGINE_GAME_INL_INCLUDED
