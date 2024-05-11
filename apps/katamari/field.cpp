#include "field.hpp"

Field::Field(borov_engine::Game& game) : BoxComponent(game, 2.0f, 0.0f, 2.0f) {
    LoadTexture("resources/textures/leaves.jpg");
}
