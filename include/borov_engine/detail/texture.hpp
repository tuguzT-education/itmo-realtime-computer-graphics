#pragma once

#ifndef BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED
#define BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED

#include <d3d11.h>

#include <filesystem>

#include "d3d_ptr.hpp"

namespace borov_engine::detail {

[[nodiscard]] D3DPtr<ID3D11ShaderResourceView> TextureFromFile(ID3D11Device &device, const std::filesystem::path &path);

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED
