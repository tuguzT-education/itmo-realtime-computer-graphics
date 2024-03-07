#include <borov_engine/game.hpp>
#include <borov_engine/camera.hpp>
#include <borov_engine/box_component.hpp>

class Game : public borov_engine::Game {
  public:
    explicit Game(borov_engine::Window &window, borov_engine::Input &input)
        : borov_engine::Game(window, input) {}

    void Update(float delta_time) override {
        auto camera = Camera();
        auto input = Input();
        if (camera != nullptr && input != nullptr) {
            using borov_engine::InputKey;
            auto x = static_cast<float>(input->IsKeyDown(InputKey::D) - input->IsKeyDown(InputKey::A));
            auto y = static_cast<float>(input->IsKeyDown(InputKey::Space) - input->IsKeyDown(InputKey::LeftControl));
            auto z = static_cast<float>(input->IsKeyDown(InputKey::W) - input->IsKeyDown(InputKey::S));
            borov_engine::math::Vector3 direction = camera->Right() * x + camera->Up() * y + camera->Forward() * z;
            float speed = input->IsKeyDown(InputKey::LeftShift) ? 2.0f : 1.0f;
            camera->Position() += direction * speed * delta_time;
        }

        borov_engine::Game::Update(delta_time);
    }
};

int main() {
    borov_engine::Window window{"Solar system", 800, 800};
    borov_engine::Input input{window};
    Game game{window, input};

    game.AddComponent<borov_engine::BoxComponent>(1.0f, 1.0f, 1.0f);

    game.Run();
    return 0;
}
