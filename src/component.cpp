#include "borov_engine/component.hpp"

namespace borov_engine {

Component::Component(Game &game) : game_{game} {}

Component::~Component() = default;

Input *Component::Input() {
    return &game_.get().input_;
}

const Input *Component::Input() const {
    return &game_.get().input_;
}

Window *Component::Window() {
    return &game_.get().window_;
}

const Window *Component::Window() const {
    return &game_.get().window_;
}

ID3D11DeviceContext *Component::DeviceContext() {
    return game_.get().device_context_.Get();
}

const ID3D11DeviceContext *Component::DeviceContext() const {
    return game_.get().device_context_.Get();
}

ID3D11Device *Component::Device() {
    return game_.get().device_.Get();
}

const ID3D11Device *Component::Device() const {
    return game_.get().device_.Get();
}

const math::Color &Component::ClearColor() const {
    return game_.get().ClearColor();
}

math::Color & Component::ClearColor() {
    return game_.get().ClearColor();
}

const Timer &Component::Timer() const {
    return game_.get().timer_;
}

}
