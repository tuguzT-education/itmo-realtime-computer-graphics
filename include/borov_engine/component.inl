#pragma once

#ifndef BOROV_ENGINE_COMPONENT_INL_INCLUDED
#define BOROV_ENGINE_COMPONENT_INL_INCLUDED

namespace borov_engine {

ComponentConstView auto Component::Components() const {
    const Game &game = game_.get();
    return game.Components();
}

ComponentView auto Component::Components() {
    return game_.get().Components();
}

}

#endif //BOROV_ENGINE_COMPONENT_INL_INCLUDED
