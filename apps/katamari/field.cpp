#include "field.hpp"

Field::Field(borov_engine::Game &game)
    : BoxComponent(game, Initializer{
                             .length = 100.0f,
                             .height = 0.0f,
                             .width = 100.0f,
                             .texture_path = "resources/textures/leaves.jpg",
                             .tile_count = borov_engine::math::Vector2::One * 100.0f,
                         }) {}
