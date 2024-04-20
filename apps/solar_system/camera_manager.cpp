#include "camera_manager.hpp"

#include <borov_engine/game.hpp>

CameraManager::CameraManager(borov_engine::Game &game, const float yaw, const float pitch)
    : borov_engine::CameraManager(game), wheel_delta_{}, yaw_{yaw}, pitch_{pitch} {
    camera_.Rotation() = borov_engine::math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
    camera_.ProjectionType() = borov_engine::CameraProjectionType::Perspective;

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
    return &camera_;
}

borov_engine::Camera *CameraManager::MainCamera() {
    return &camera_;
}

float CameraManager::Yaw() const {
    return yaw_;
}

void CameraManager::Yaw(const float yaw) {
    yaw_ = yaw;
}

float CameraManager::Pitch() const {
    return pitch_;
}

void CameraManager::Pitch(const float pitch) {
    pitch_ = pitch;
}

void CameraManager::Update(const float delta_time) {
    if (const auto input = Game().Input(); input != nullptr) {
        using borov_engine::InputKey;
        namespace math = borov_engine::math;

        const auto x = static_cast<float>(input->IsKeyDown(InputKey::D) - input->IsKeyDown(InputKey::A));
        const auto y = static_cast<float>(input->IsKeyDown(InputKey::Space) - input->IsKeyDown(InputKey::LeftControl));
        const auto z = static_cast<float>(input->IsKeyDown(InputKey::W) - input->IsKeyDown(InputKey::S));
        const math::Vector3 direction = camera_.Right() * x + math::Vector3::Up * y + camera_.Forward() * z;
        const float speed = input->IsKeyDown(InputKey::LeftShift) ? 2.0f : 1.0f;
        camera_.Position() += direction * speed * delta_time;

        yaw_ -= mouse_offset_.x * 0.005f;
        pitch_ -= mouse_offset_.y * 0.005f;
        camera_.Rotation() = math::Quaternion::CreateFromYawPitchRoll(yaw_, pitch_, 0.0f);
        mouse_offset_ = math::Vector2::Zero;

        const float fov = camera_.HorizontalFOV() + static_cast<float>(-wheel_delta_) * 0.005f;
        camera_.HorizontalFOV(fov);
        wheel_delta_ = 0;
    }
}

// ReSharper disable once CppPassValueParameterByConstReference
void CameraManager::OnMouseMove(const borov_engine::MouseMoveData data) {
    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}
