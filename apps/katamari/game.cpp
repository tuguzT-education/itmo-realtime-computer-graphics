#include "game.hpp"

#include <borov_engine/camera.hpp>
#include <borov_engine/orbit_camera_manager.hpp>

#include "viewport_manager.hpp"

Game::Game(borov_engine::Window &window, borov_engine::Input &input)
    : borov_engine::Game(window, input),
      camera_{
          AddComponent<borov_engine::Camera>([] {
              borov_engine::Camera::Initializer initializer{
                  .projection = std::make_unique<borov_engine::PerspectiveProjection>(),
                  .far_plane = 10.0f,
              };
              initializer.transform = {.position = borov_engine::math::Vector3::Backward};
              return initializer;
          }()),
      },
      field_{AddComponent<Field>()},
      player_{AddComponent<Player>()},
      apricot_{
          AddComponent<Apricot>(Apricot::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Right * 6.0f},
          }),
      },
      axe_{
          AddComponent<Axe>(Axe::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Left * 3.0f},
          }),
      },
      boat_{
          AddComponent<Boat>(Boat::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Backward * 9.0f},
          }),
      },
      bulb_{
          AddComponent<Bulb>(Bulb::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Backward * 6.0f},
          }),
      },
      cake_{
          AddComponent<Cake>(Cake::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Right * 9.0f},
          }),
      },
      chair_{
          AddComponent<Chair>(Chair::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Left * 6.0f},
          }),
      },
      cheese_{
          AddComponent<Cheese>(Cheese::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Forward * 6.0f},
          }),
      },
      concrete_barricade_{
          AddComponent<ConcreteBarricade>(ConcreteBarricade::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Left * 9.0f},
          }),
      },
      die_{
          AddComponent<Die>(Die::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Forward * 3.0f},
          }),
      },
      hog_{
          AddComponent<Hog>(Hog::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Forward * 9.0f},
          }),
      },
      strawberry_{
          AddComponent<Strawberry>(Strawberry::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Right * 3.0f},
          }),
      },
      tanto_{
          AddComponent<Tanto>(Tanto::Initializer{
              .transform = borov_engine::Transform{.position = borov_engine::math::Vector3::Backward * 3.0f},
          }),
      } {
    using borov_engine::OrbitCameraManager;
    CameraManager<OrbitCameraManager>(OrbitCameraManager::Initializer{
        .target = player_,
        .camera = &camera_.get(),
        .zoom_speed = 10.0f,
    });
    ViewportManager<::ViewportManager>();

    namespace math = borov_engine::math;

    ClearColor() = math::colors::linear::SkyBlue;

    DirectionalLight().IsLightEnabled() = true;
    DirectionalLight().Ambient() = math::Color{math::colors::linear::White} * 0.1f;
    DirectionalLight().Diffuse() = math::Color{math::colors::linear::White} * 0.5f;
    DirectionalLight().Specular() = math::Color{math::colors::linear::White} * 0.5f;
    DirectionalLight().Direction(math::Normalize(math::Vector3::Down + math::Vector3::Forward));

    PointLight().IsLightEnabled() = true;
    PointLight().Ambient() = math::Color{math::colors::linear::Black};
    PointLight().Diffuse() = math::Color{math::colors::linear::White};
    PointLight().Specular() = math::Color{math::colors::linear::White};
    PointLight().Attenuation().linear_factor = math::Color{math::colors::linear::White};
    PointLight().Parent(&bulb_.get());
    PointLight().Transform() = borov_engine::Transform{.position = math::Vector3::Up * 0.2f};

    SpotLight().IsLightEnabled() = true;
    SpotLight().Ambient() = math::Color{math::colors::linear::Black};
    SpotLight().Diffuse() = math::Color{math::colors::linear::White};
    SpotLight().Specular() = math::Color{math::colors::linear::White};
    SpotLight().Transform() = borov_engine::Transform{.position = math::Vector3::Up * 2.0f};
    SpotLight().Direction(math::Vector3::Down);
    SpotLight().Attenuation().linear_factor = math::Color{math::colors::linear::White};
    SpotLight().InnerConeAngle() = std::numbers::pi_v<float> / 6.0f;
    SpotLight().OuterConeAngle() = std::numbers::pi_v<float> / 3.0f;
}

void Game::Update(const float delta_time) {
    borov_engine::Game::Update(delta_time);

    namespace math = borov_engine::math;

    const borov_engine::Input *input = Input();
    if (input == nullptr) {
        return;
    }

    const borov_engine::Transform camera_transform = camera_.get().WorldTransform();

    auto [left_key, right_key, forward_key, backward_key] = player_.get().Controls();
    const auto x = static_cast<float>(input->IsKeyDown(right_key) - input->IsKeyDown(left_key));
    const auto z = static_cast<float>(input->IsKeyDown(forward_key) - input->IsKeyDown(backward_key));

    const math::Vector3 right = camera_transform.Right();
    math::Vector3 forward = camera_transform.Forward();
    forward.y = 0.0f;
    forward.Normalize();
    if (forward.LengthSquared() <= 0.1f) {
        forward = right.Cross(math::Vector3::Up);
    }

    math::Vector3 direction = right * x + forward * z;
    direction.y = 0.0f;
    direction.Normalize();

    borov_engine::Transform player_transform = player_.get().WorldTransform();
    player_transform.position += direction * delta_time;
    if (direction.LengthSquared() > 0.1f) {
        const auto additional = math::Quaternion::CreateFromAxisAngle(-direction.Cross(math::Vector3::Up),
                                                                      std::numbers::pi_v<float> * 0.5f * delta_time);
        player_transform.rotation = math::Quaternion::Concatenate(additional, player_transform.rotation);
    }
    player_.get().WorldTransform(player_transform);

    // ReSharper disable once CppTooWideScopeInitStatement
    auto is_scene_and_collision = [](const borov_engine::Component &component) {
        return dynamic_cast<const borov_engine::SceneComponent *>(&component) != nullptr &&
               dynamic_cast<const borov_engine::Collision *>(&component) != nullptr;
    };
    for (borov_engine::Component &component : Components() | std::views::filter(is_scene_and_collision)) {
        if (&component == &player_.get() || &component == &field_.get()) {
            continue;
        }

        auto &scene_component = dynamic_cast<borov_engine::SceneComponent &>(component);
        auto &collision_primitive = dynamic_cast<const borov_engine::Collision &>(component);

        if (collision_primitive.Intersects(player_)) {
            scene_component.Parent(&player_.get());
        }
    }

    const borov_engine::Window *window = Window();
    if (window != nullptr && input->IsKeyDown(borov_engine::InputKey::LeftButton)) {
        const math::Vector3 world_cursor_position = ScreenToWorld(window->CursorPosition());
        if (std::isnan(world_cursor_position.LengthSquared())) {
            return;
        }
        const math::Vector3 ray_position = camera_.get().WorldTransform().position;
        const math::Vector3 ray_direction = math::Normalize(world_cursor_position - ray_position);
        const math::Ray ray{ray_position, ray_direction};

        const borov_engine::Collision *target = nullptr;
        float nearest_distance = std::numeric_limits<float>::infinity();

        auto is_collision = [](const borov_engine::Component &component) {
            return dynamic_cast<const borov_engine::Collision *>(&component) != nullptr;
        };
        for (borov_engine::Component &component : Components() | std::views::filter(is_collision)) {
            auto &collision = dynamic_cast<const borov_engine::Collision &>(component);
            if (float distance = 100.0f; collision.Intersects(ray, distance)) {
                if (target == nullptr || distance < nearest_distance) {
                    target = &collision;
                    nearest_distance = distance;
                }
            }
        }

        if (target != nullptr) {
            const math::Vector3 hit_position = ray_position + ray_direction * nearest_distance;
            const math::Vector3 light_direction = math::Normalize(hit_position - SpotLight().WorldTransform().position);
            SpotLight().Direction(light_direction);

            const borov_engine::Transform debug_transform{.position = hit_position};
            DebugDraw().DrawPivot(debug_transform, {.duration = delta_time});
        }
    }
}

void Game::Draw(const borov_engine::Camera *camera) {
    borov_engine::Game::Draw(camera);

    DebugDraw().DrawPivot(SpotLight().WorldTransform());
}
