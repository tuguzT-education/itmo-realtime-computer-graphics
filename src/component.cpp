#include "borov_engine/component.hpp"

#include "borov_engine/game.hpp"

namespace borov_engine {

Component::Component(borov_engine::Game &game) : game_{game} {}

Component::~Component() = default;

void Component::Update(float delta_time) {}

void Component::Draw(const Camera *camera) {}

void Component::OnTargetResize() {}

Game &Component::Game() {
    return game_;
}

const Game &Component::Game() const {
    return game_;
}

ID3D11DeviceContext &Component::DeviceContext() {
    return *Game().device_context_.Get();
}

const ID3D11DeviceContext &Component::DeviceContext() const {
    return *Game().device_context_.Get();
}

ID3D11Device &Component::Device() {
    return *Game().device_.Get();
}

const ID3D11Device &Component::Device() const {
    return *Game().device_.Get();
}

}
