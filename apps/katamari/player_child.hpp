#pragma once

#ifndef KATAMARI_PLAYER_CHILD_HPP_INCLUDED
#define KATAMARI_PLAYER_CHILD_HPP_INCLUDED

#include <borov_engine/triangle_component.hpp>

class PlayerChild final : public borov_engine::TriangleComponent {
  public:
    explicit PlayerChild(borov_engine::Game& game, const Initializer& initializer = {});

  private:
    struct alignas(16) VertexShaderConstantBuffer : TriangleComponent::VertexShaderConstantBuffer {
        float time = 0.0f;
    };

    void InitializeVertexShader() override;
    void InitializeVertexShaderConstantBuffer() override;
    void UpdateVertexShaderConstantBuffer(const TriangleComponent::VertexShaderConstantBuffer& data) override;
};

#endif  // KATAMARI_PLAYER_CHILD_HPP_INCLUDED
