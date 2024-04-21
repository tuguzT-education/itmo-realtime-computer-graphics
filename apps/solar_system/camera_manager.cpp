#include "camera_manager.hpp"

#include <algorithm>
#include <borov_engine/game.hpp>

CameraManager::CameraManager(borov_engine::Game &game, borov_engine::Camera &camera)
    : borov_engine::CameraManager(game), wheel_delta_{}, camera_{camera} {
    camera.ProjectionType() = borov_engine::PerspectiveCameraProjectionType{};

    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().AddRaw(this, &CameraManager::OnMouseMove);
    }
}

CameraManager::~CameraManager() {
    if (const auto input = Game().Input(); input != nullptr) {
        input->OnMouseMove().RemoveByOwner(this);
    }
}

const borov_engine::Camera *CameraManager::MainCamera() const {
    return &camera_.get();
}

borov_engine::Camera *CameraManager::MainCamera() {
    return &camera_.get();
}

void CameraManager::Update(const float delta_time) {
    if (const auto input = Game().Input(); input != nullptr) {
        using borov_engine::InputKey;
        namespace math = borov_engine::math;

        const auto x = static_cast<float>(input->IsKeyDown(InputKey::D) - input->IsKeyDown(InputKey::A));
        const auto y = static_cast<float>(input->IsKeyDown(InputKey::Space) - input->IsKeyDown(InputKey::LeftControl));
        const auto z = static_cast<float>(input->IsKeyDown(InputKey::W) - input->IsKeyDown(InputKey::S));

        borov_engine::Transform &transform = camera_.get().Transform();
        const math::Vector3 direction = transform.Right() * x + math::Vector3::Up * y + transform.Forward() * z;
        const float speed = input->IsKeyDown(InputKey::LeftShift) ? 2.0f : 1.0f;
        transform.position += direction * speed * delta_time;

        auto [pitch, yaw, roll] = transform.rotation.ToEuler();
        yaw -= mouse_offset_.x * 0.005f;
        pitch -= mouse_offset_.y * 0.005f;

        constexpr float max_pitch = std::numbers::pi_v<float> / 2.0f - 0.001f;
        pitch = std::clamp(pitch, -max_pitch, max_pitch);

        transform.rotation = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
        mouse_offset_ = math::Vector2::Zero;

        if (borov_engine::CameraProjectionType &projection_type = camera_.get().ProjectionType();
            std::holds_alternative<borov_engine::PerspectiveCameraProjectionType>(projection_type)) {
            auto &[horizontal_fov] = std::get<borov_engine::PerspectiveCameraProjectionType>(projection_type);

            horizontal_fov -= static_cast<float>(wheel_delta_) * 0.005f;
            wheel_delta_ = 0;
        }
    }
}

void CameraManager::OnMouseMove(const borov_engine::MouseMoveData &data) {
    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}
