#include "field.hpp"

Field::Field(borov_engine::Game &game)
    : BoxComponent(game, [] {
          Initializer initializer{
              .length = 100.0f,
              .height = 0.0f,
              .width = 100.0f,
          };
          initializer.texture_path = "resources/textures/leaves.jpg";
          initializer.tile_count = borov_engine::math::Vector2::One * 100.0f;
          return initializer;
      }()) {}
