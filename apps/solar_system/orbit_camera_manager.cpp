#include "orbit_camera_manager.hpp"

#include <algorithm>
#include <borov_engine/game.hpp>

OrbitCameraManager::OrbitCameraManager(borov_engine::Game& game, borov_engine::Camera& camera,
                                       const borov_engine::SceneComponent& target)
    : CameraManager(game),
      wheel_delta_{},
      distance_{(camera.Transform().position - target.WorldTransform().position).Length()},
      camera_{camera},
      target_{target} {
    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().AddRaw(this, &OrbitCameraManager::OnMouseMove);
    }
}

OrbitCameraManager::~OrbitCameraManager() {
    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().RemoveByOwner(this);
    }
}

const borov_engine::Camera* OrbitCameraManager::MainCamera() const {
    return &camera_;
}

borov_engine::Camera* OrbitCameraManager::MainCamera() {
    return &camera_;
}

void OrbitCameraManager::Update(const float delta_time) {
    if (const auto input = Game().Input(); input != nullptr) {
        using borov_engine::InputKey;
        namespace math = borov_engine::math;

        borov_engine::Transform& transform = camera_.Transform();

        auto [pitch, yaw, roll] = transform.rotation.ToEuler();
        yaw -= mouse_offset_.x * 0.005f;
        pitch -= mouse_offset_.y * 0.005f;

        constexpr float max_pitch = std::numbers::pi_v<float> / 2.0f - 0.001f;
        pitch = std::clamp(pitch, -max_pitch, max_pitch);

        transform.rotation = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
        transform.position = target_.WorldTransform().position - transform.Forward() * distance_;
        mouse_offset_ = math::Vector2::Zero;

        if (borov_engine::CameraProjectionType& projection_type = camera_.ProjectionType();
            std::holds_alternative<borov_engine::PerspectiveCameraProjectionType>(projection_type)) {
            auto& [horizontal_fov] = std::get<borov_engine::PerspectiveCameraProjectionType>(projection_type);

            if (input->IsKeyDown(InputKey::MiddleButton)) {
                distance_ -= static_cast<float>(wheel_delta_) * 0.1f;
            } else {
                horizontal_fov -= static_cast<float>(wheel_delta_) * 0.005f;
            }
            wheel_delta_ = 0;
        }
    }
}

void OrbitCameraManager::OnMouseMove(const borov_engine::MouseMoveData& data) {
    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}
