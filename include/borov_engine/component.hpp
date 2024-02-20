#pragma once

#ifndef BOROV_ENGINE_CONTEXT_HPP_INCLUDED
#define BOROV_ENGINE_CONTEXT_HPP_INCLUDED

#include "game.fwd"

namespace borov_engine {

class Component {
  public:
    explicit Component(Game &game) : game_{game} {}
    virtual ~Component() = default;

  private:
    Game &game_;
};

}

#endif //BOROV_ENGINE_CONTEXT_HPP_INCLUDED
