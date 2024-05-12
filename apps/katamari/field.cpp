#include "field.hpp"

Field::Field(borov_engine::Game &game)
    : BoxComponent(game, Initializer{.length = 2.0f, .height = 0.0f, .width = 2.0f}) {
    LoadTexture("resources/textures/leaves.jpg");
}
