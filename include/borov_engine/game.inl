#pragma once

#ifndef BOROV_ENGINE_GAME_INL_INCLUDED
#define BOROV_ENGINE_GAME_INL_INCLUDED

namespace borov_engine {

template<typename T, typename... Args>
void Game::AddComponent(Args &&... args) {
    auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
    components_.push_back(std::move(component));
}

}

#endif //BOROV_ENGINE_GAME_INL_INCLUDED
