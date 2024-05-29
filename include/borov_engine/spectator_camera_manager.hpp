#pragma once

#ifndef BOROV_ENGINE_SPECTATOR_CAMERA_MANAGER_HPP_INCLUDED
#define BOROV_ENGINE_SPECTATOR_CAMERA_MANAGER_HPP_INCLUDED

#include "camera_manager.hpp"
#include "input.hpp"

namespace borov_engine {

class SpectatorCameraManager : public CameraManager {
  public:
    struct MovementInput {
        struct Axis {
            InputKey positive = InputKey::None;
            InputKey negative = InputKey::None;
        };

        Axis x{
            .positive = InputKey::D,
            .negative = InputKey::A,
        };
        Axis y{
            .positive = InputKey::Space,
            .negative = InputKey::LeftControl,
        };
        Axis z{
            .positive = InputKey::W,
            .negative = InputKey::S,
        };
    };

    struct Initializer : CameraManager::Initializer {
        Camera* camera = nullptr;
        MovementInput movement_input;
        InputKey enable_look_input_key = InputKey::MiddleButton;
        float speed = 1.0f;
        float sensitivity = 1.0f;
        float zoom_speed = 1.0f;
    };

    explicit SpectatorCameraManager(class Game& game, const Initializer& initializer = {});
    ~SpectatorCameraManager() override;

    [[nodiscard]] MovementInput MovementInput() const;
    [[nodiscard]] class MovementInput& MovementInput();

    [[nodiscard]] InputKey EnableLookInputKey() const;
    [[nodiscard]] InputKey &EnableLookInputKey();

    [[nodiscard]] float Speed() const;
    bool Speed(float speed);

    [[nodiscard]] float Sensitivity() const;
    bool Sensitivity(float sensitivity);

    [[nodiscard]] float ZoomSpeed() const;
    bool ZoomSpeed(float zoom_speed);

    [[nodiscard]] const Camera* MainCamera() const override;
    [[nodiscard]] Camera* MainCamera() override;

    void Update(float delta_time) override;

  private:
    void OnMouseMove(const MouseMoveData& data);

    std::reference_wrapper<Camera> camera_;

    class MovementInput movement_input_;
    InputKey enable_look_input_key_;
    float speed_;
    float sensitivity_;
    float zoom_speed_;

    math::Vector2 mouse_offset_;
    std::int32_t wheel_delta_;
};

}  // namespace borov_engine

#endif  // BOROV_ENGINE_SPECTATOR_CAMERA_MANAGER_HPP_INCLUDED
