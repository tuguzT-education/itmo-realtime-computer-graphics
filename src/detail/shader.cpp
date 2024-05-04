#include "borov_engine/detail/shader.hpp"

#include <d3dcompiler.h>

#include <format>

#include "borov_engine/detail/check_result.hpp"

namespace borov_engine::detail {

D3DPtr<ID3DBlob> ShaderFromFile(const std::filesystem::path& path, const D3D_SHADER_MACRO* defines,
                                ID3DInclude* include, const char* entrypoint, const char* target, const UINT flags_1,
                                const UINT flags_2) {
    D3DPtr<ID3DBlob> shader;

    D3DPtr<ID3DBlob> error_messages;
    const HRESULT result = D3DCompileFromFile(path.c_str(), defines, include, entrypoint, target, flags_1, flags_2,
                                              &shader, &error_messages);
    CheckResult(result, [&] {
        const char* message = error_messages ? static_cast<const char*>(error_messages->GetBufferPointer())
                                             : "file is missing";
        return std::format("Failed to compile shader from file '{}':\n{}", path.generic_string(), message);
    });

    return shader;
}

}  // namespace borov_engine::detail
