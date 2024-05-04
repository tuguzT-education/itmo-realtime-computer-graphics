#pragma once

#ifndef BOROV_ENGINE_DETAIL_SHADER_HPP_INCLUDED
#define BOROV_ENGINE_DETAIL_SHADER_HPP_INCLUDED

#include <d3dcommon.h>

#include <filesystem>

#include "d3d_ptr.hpp"

namespace borov_engine::detail {

[[nodiscard]] D3DPtr<ID3DBlob> ShaderFromFile(const std::filesystem::path &path, const D3D_SHADER_MACRO *defines,
                                              ID3DInclude *include, const char *entrypoint, const char *target,
                                              UINT flags_1, UINT flags_2);

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_DETAIL_SHADER_HPP_INCLUDED
