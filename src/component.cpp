#include "borov_engine/component.hpp"

#include "borov_engine/game.hpp"

namespace borov_engine {

Component::Component(Game &game) : game_{game} {}

ID3D11DeviceContext *Component::GetDeviceContext() {
    return game_.get().device_context_.Get();
}

ID3D11Device *Component::GetDevice() {
    return game_.get().device_.Get();
}

}
