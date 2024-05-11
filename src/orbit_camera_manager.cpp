#include "borov_engine/orbit_camera_manager.hpp"

#include "borov_engine/camera.hpp"
#include "borov_engine/game.hpp"

namespace borov_engine {

OrbitCameraManager::OrbitCameraManager(class Game& game, const SceneComponent& target, const float distance,
                                       const float sensitivity, const float zoom_speed)
    : OrbitCameraManager(game, game.AddComponent<Camera>(PerspectiveCameraProjectionType{}), target, distance,
                         sensitivity, zoom_speed) {}

OrbitCameraManager::OrbitCameraManager(class Game& game, Camera& camera, const SceneComponent& target,
                                       const float distance, const float sensitivity, const float zoom_speed)
    : CameraManager(game),
      wheel_delta_{},
      distance_{distance > 0.0f ? distance : (camera.Transform().position - target.WorldTransform().position).Length()},
      sensitivity_{sensitivity > 0.0f ? sensitivity : 1.0f},
      zoom_speed_{zoom_speed > 0.0f ? zoom_speed : 1.0f},
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

float OrbitCameraManager::Distance() const {
    return distance_;
}

bool OrbitCameraManager::Distance(const float distance) {
    if (distance < 0.0f) {
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

    auto [pitch, yaw, roll] = transform.rotation.ToEuler();
    yaw -= mouse_offset_.x * sensitivity_ * delta_time;
    pitch -= mouse_offset_.y * sensitivity_ * delta_time;

    constexpr float max_pitch = std::numbers::pi_v<float> / 2.0f - 0.001f;
    constexpr float min_pitch = -max_pitch;
    pitch = std::clamp(pitch, min_pitch, max_pitch);

    transform.rotation = math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
    transform.position = target_.get().WorldTransform().position - transform.Forward() * distance_;
    mouse_offset_ = math::Vector2::Zero;

    distance_ -= static_cast<float>(wheel_delta_) * zoom_speed_ * delta_time;
    if (distance_ < 1e-3f) {
        distance_ = 1e-3f;
    }
    wheel_delta_ = 0;
}

void OrbitCameraManager::OnMouseMove(const MouseMoveData& data) {
    mouse_offset_ += data.offset;
    wheel_delta_ += data.wheel_delta;
}

}  // namespace borov_engine
