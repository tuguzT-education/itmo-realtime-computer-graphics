#include <borov_engine/game.hpp>
#include <borov_engine/camera.hpp>
#include <borov_engine/box_component.hpp>

class Game : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input)
        : borov_engine::Game(window, input), wheel_delta_{} {
        input.OnMouseMove().AddRaw(this, &Game::OnMouseMove);

        auto camera = Camera();
        if (camera != nullptr) {
            camera->ProjectionType() = borov_engine::CameraProjectionType::Perspective;
        }
    }

    ~Game() override {
        auto input = Input();
        if (input != nullptr) {
            input->OnMouseMove().RemoveByOwner(this);
        }
    }

    void Update(float delta_time) override {
        auto camera = Camera();
        auto input = Input();
        if (camera != nullptr && input != nullptr) {
            using borov_engine::InputKey;
            namespace math = borov_engine::math;

            auto x = static_cast<float>(input->IsKeyDown(InputKey::D) - input->IsKeyDown(InputKey::A));
            auto y = static_cast<float>(input->IsKeyDown(InputKey::Space) - input->IsKeyDown(InputKey::LeftControl));
            auto z = static_cast<float>(input->IsKeyDown(InputKey::W) - input->IsKeyDown(InputKey::S));
            math::Vector3 direction = camera->Right() * x + math::Vector3::Up * y + camera->Forward() * z;
            float speed = input->IsKeyDown(InputKey::LeftShift) ? 2.0f : 1.0f;
            camera->Position() += direction * speed * delta_time;

            float yaw = -mouse_offset_.x * 0.005f;
            float pitch = -mouse_offset_.y * 0.005f;
            camera->Rotation(yaw, pitch, 0.0f);

            float fov = camera->HorizontalFOV() * (1 + static_cast<float>(-wheel_delta_) * 0.05f);
            camera->HorizontalFOV(fov);
            wheel_delta_ = 0;
        }

        borov_engine::Game::Update(delta_time);
    }

  private:
    void OnMouseMove(borov_engine::MouseMoveData data) {
        mouse_offset_ += data.offset;
        wheel_delta_ += data.wheel_delta;
    }

    borov_engine::math::Vector2 mouse_offset_;
    std::int32_t wheel_delta_;
};

int main() {
    borov_engine::Window window{"Solar system", 800, 800};
    borov_engine::Input input{window};
    Game game{window, input};

    game.AddComponent<borov_engine::BoxComponent>(1.0f, 1.0f, 1.0f);

    game.Run();
    return 0;
}
