#include "player.hpp"

Player::Player(borov_engine::Game& game, const ControlKeys control_keys)
    : BoxComponent(game, 0.5f, 0.5f, 0.5f, borov_engine::math::colors::linear::White.v,
                   "resources/textures/cardboard.jpg"),
      control_keys_{control_keys} {}
