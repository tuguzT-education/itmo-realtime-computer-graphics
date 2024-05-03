#pragma once

#ifndef BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED
#define BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED

#include <d3d11.h>

#include <string_view>

#include "d3d_ptr.hpp"

namespace borov_engine::detail {

[[nodiscard]] D3DPtr<ID3D11ShaderResourceView> TextureFromFile(ID3D11Device &device, std::string_view path);

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_DETAIL_TEXTURE_HPP_INCLUDED
