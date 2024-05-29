#include "borov_engine/orbit_camera_manager.hpp"

#include <algorithm>
#include <iostream>

#include "borov_engine/camera.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

constexpr float OrbitCameraManager::min_distance = 1e-4f;

OrbitCameraManager::OrbitCameraManager(class Game& game, const Initializer& initializer)
    : CameraManager(game),
      camera_{
          initializer.camera
              ? *initializer.camera
              : game.AddComponent<Camera>(Camera::Initializer{.projection = std::make_unique<PerspectiveProjection>()}),
      },
      target_{initializer.target},
      enable_look_input_key_{initializer.enable_look_input_key},
      distance_{initializer.distance > min_distance
                    ? initializer.distance
                    : (camera_.get().Transform().position - target_.get().WorldTransform().position).Length()},
      sensitivity_{initializer.sensitivity},
      zoom_speed_{initializer.zoom_speed},
      wheel_delta_{} {
    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().AddRaw(this, &OrbitCameraManager::OnMouseMove);
    }
}

OrbitCameraManager::~OrbitCameraManager() {
    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().RemoveByOwner(this);
    }
}

InputKey OrbitCameraManager::EnableLookInputKey() const {
    return enable_look_input_key_;
}

InputKey& OrbitCameraManager::EnableLookInputKey() {
    return enable_look_input_key_;
}

float OrbitCameraManager::Distance() const {
    return distance_;
}

bool OrbitCameraManager::Distance(const float distance) {
    if (distance < min_distance) {
        return false;
    }
    distance_ = distance;
    return true;
}

float OrbitCameraManager::Sensitivity() const {
    return sensitivity_;
}

bool OrbitCameraManager::Sensitivity(const float sensitivity) {
    if (sensitivity < 0.0f) {
        return false;
    }
    sensitivity_ = sensitivity;
    return true;
}

float OrbitCameraManager::ZoomSpeed() const {
    return zoom_speed_;
}

bool OrbitCameraManager::ZoomSpeed(const float zoom_speed) {
    if (zoom_speed < 0.0f) {
        return false;
    }
    zoom_speed_ = zoom_speed;
    return true;
}

const Camera* OrbitCameraManager::MainCamera() const {
    return &camera_.get();
}

Camera* OrbitCameraManager::MainCamera() {
    return &camera_.get();
}

void OrbitCameraManager::Update(const float delta_time) {
    CameraManager::Update(delta_time);

    Transform& transform = camera_.get().Transform();
    if (const Input* input = Game().Input();
        enable_look_input_key_ == InputKey::None || (input != nullptr && input->IsKeyDown(enable_look_input_key_))) {
        auto [pitch, yaw, roll] = transform.rotation.ToEuler();
        yaw -= mouse_offset_.x * sensitivity_ * delta_time;
        pitch -= mouse_offset_.y * sensitivity_ * delta_time;

        constexpr float max_pitch = std::numbers::pi_v<float> / 2.0f - 0.001f;
        constexpr float min_pitch = -max_pitch;
        pitch = std::clamp(pitch, min_pitch, max_pitch);

        transform.rotation = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);

        distance_ -= static_cast<float>(wheel_delta_) * zoom_speed_ * delta_time;
        if (distance_ < min_distance) {
            distance_ = min_distance;
        }
    }
    transform.position = target_.get().WorldTransform().position - transform.Forward() * distance_;

    mouse_offset_ = math::Vector2::Zero;
    wheel_delta_ = 0;
}

void OrbitCameraManager::OnMouseMove(const MouseMoveData& data) {
    if (const Window* window = Game().Window(); window == nullptr || !window->IsCursorInWindow()) {
        return;
    }

    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}

}  // namespace borov_engine
