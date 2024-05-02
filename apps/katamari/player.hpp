#pragma once

#ifndef KATAMARI_PLAYER_HPP_INCLUDED
#define KATAMARI_PLAYER_HPP_INCLUDED

#include <borov_engine/box_component.hpp>
#include <borov_engine/input.hpp>

class Player final : public borov_engine::BoxComponent {
  public:
    struct ControlKeys {
        using enum borov_engine::InputKey;

        borov_engine::InputKey left = A;
        borov_engine::InputKey right = D;
        borov_engine::InputKey forward = W;
        borov_engine::InputKey backward = S;
    };

    explicit Player(borov_engine::Game &game, ControlKeys control_keys = {});

  private:
    ControlKeys control_keys_;
};

#endif  // KATAMARI_PLAYER_HPP_INCLUDED
