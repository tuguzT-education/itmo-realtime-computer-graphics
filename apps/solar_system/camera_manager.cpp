#include "camera_manager.hpp"

CameraManager::CameraManager(borov_engine::Game &game) : borov_engine::CameraManager(game), wheel_delta_{} {
    camera_.ProjectionType() = borov_engine::CameraProjectionType::Perspective;

    auto input = Game().Input();
    if (input != nullptr) {
        input->OnMouseMove().AddRaw(this, &CameraManager::OnMouseMove);
    }
}

CameraManager::~CameraManager() {
    auto input = Game().Input();
    if (input != nullptr) {
        input->OnMouseMove().RemoveByOwner(this);
    }
}

const borov_engine::Camera *CameraManager::MainCamera() const {
    return &camera_;
}

borov_engine::Camera *CameraManager::MainCamera() {
    return &camera_;
}

void CameraManager::Update(float delta_time) {
    auto input = Game().Input();
    if (input != nullptr) {
        using borov_engine::InputKey;
        namespace math = borov_engine::math;

        auto x = static_cast<float>(input->IsKeyDown(InputKey::D) - input->IsKeyDown(InputKey::A));
        auto y = static_cast<float>(input->IsKeyDown(InputKey::Space) - input->IsKeyDown(InputKey::LeftControl));
        auto z = static_cast<float>(input->IsKeyDown(InputKey::W) - input->IsKeyDown(InputKey::S));
        math::Vector3 direction = camera_.Right() * x + math::Vector3::Up * y + camera_.Forward() * z;
        float speed = input->IsKeyDown(InputKey::LeftShift) ? 2.0f : 1.0f;
        camera_.Position() += direction * speed * delta_time;

        float yaw = -mouse_offset_.x * 0.005f;
        float pitch = -mouse_offset_.y * 0.005f;
        camera_.Rotation() = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);

        float fov = camera_.HorizontalFOV() + static_cast<float>(-wheel_delta_) * 0.005f;
        camera_.HorizontalFOV(fov);
        wheel_delta_ = 0;
    }
}

void CameraManager::Draw(const borov_engine::Camera *camera) {}

void CameraManager::OnMouseMove(borov_engine::MouseMoveData data) {
    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}
