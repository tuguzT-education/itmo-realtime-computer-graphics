#include "borov_engine/detail/texture.hpp"

#include <WICTextureLoader.h>

#include <format>

#include "borov_engine/detail/check_result.hpp"

namespace borov_engine::detail {

D3DPtr<ID3D11ShaderResourceView> TextureFromFile(ID3D11Device& device, const std::string_view path) {
    D3DPtr<ID3D11ShaderResourceView> texture;

    const std::wstring w_path = MultiByteToWideChar(CP_UTF8, 0, path);
    const HRESULT result = DirectX::CreateWICTextureFromFile(&device, w_path.c_str(), nullptr, &texture);
    CheckResult(result, [&] { return std::format("Failed to create texture from file '{}'", path); });

    return texture;
}

}  // namespace borov_engine::detail
