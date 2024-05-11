#pragma once

#ifndef KATAMARI_FIELD_HPP_INCLUDED
#define KATAMARI_FIELD_HPP_INCLUDED

#include <borov_engine/box_component.hpp>

class Field final : public borov_engine::BoxComponent {
  public:
    explicit Field(borov_engine::Game& game);
};

#endif  // KATAMARI_FIELD_HPP_INCLUDED
