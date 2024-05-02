#include "borov_engine/detail/shader.hpp"

#include <d3dcompiler.h>

#include <format>
#include <string>

#include "borov_engine/detail/check_result.hpp"

namespace borov_engine::detail {

D3DPtr<ID3DBlob> ShaderFromFile(const std::string_view path, const D3D_SHADER_MACRO* defines, ID3DInclude* include,
                                const char* entrypoint, const char* target, const UINT flags_1, const UINT flags_2) {
    D3DPtr<ID3DBlob> shader;

    D3DPtr<ID3DBlob> error_messages;
    const std::wstring w_path = MultiByteToWideChar(CP_UTF8, 0, path);
    const HRESULT result = D3DCompileFromFile(w_path.c_str(), defines, include, entrypoint, target, flags_1, flags_2,
                                              &shader, &error_messages);
    CheckResult(result, [&] {
        const char* message = error_messages ? static_cast<const char*>(error_messages->GetBufferPointer())
                                             : "file is missing";
        return std::format("Failed to compile shader from file '{}':\n{}", path, message);
    });

    return shader;
}

}  // namespace borov_engine::detail
