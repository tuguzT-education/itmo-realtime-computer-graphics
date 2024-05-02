#pragma once

#ifndef BOROV_ENGINE_SHADER_HPP_INCLUDED
#define BOROV_ENGINE_SHADER_HPP_INCLUDED

#include <d3dcommon.h>

#include <string_view>

#include "d3d_ptr.hpp"

namespace borov_engine::detail {

D3DPtr<ID3DBlob> ShaderFromFile(std::string_view path, const D3D_SHADER_MACRO *defines, ID3DInclude *include,
                                const char *entrypoint, const char *target, UINT flags_1, UINT flags_2);

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_SHADER_HPP_INCLUDED
