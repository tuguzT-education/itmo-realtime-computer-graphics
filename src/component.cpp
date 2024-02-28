#include "borov_engine/component.hpp"

namespace borov_engine {

Component::Component(Game &game) : game_{game} {}

Component::~Component() = default;

Input *Component::GetInput() {
    return &game_.get().input_;
}

const Input *Component::GetInput() const {
    return &game_.get().input_;
}

Window *Component::GetWindow() {
    return &game_.get().window_;
}

const Window *Component::GetWindow() const {
    return &game_.get().window_;
}

ID3D11DeviceContext *Component::GetDeviceContext() {
    return game_.get().device_context_.Get();
}

const ID3D11DeviceContext *Component::GetDeviceContext() const {
    return game_.get().device_context_.Get();
}

ID3D11Device *Component::GetDevice() {
    return game_.get().device_.Get();
}

const ID3D11Device *Component::GetDevice() const {
    return game_.get().device_.Get();
}

const Color &Component::ClearScreenColor() const {
    return game_.get().ClearScreenColor();
}

Color &Component::ClearScreenColor() {
    return game_.get().ClearScreenColor();
}

const Timer &Component::Timer() const {
    return game_.get().timer_;
}

}
