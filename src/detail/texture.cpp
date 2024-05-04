#include "borov_engine/detail/texture.hpp"

#include <WICTextureLoader.h>

#include <format>

#include "borov_engine/detail/check_result.hpp"

namespace borov_engine::detail {

D3DPtr<ID3D11ShaderResourceView> TextureFromFile(ID3D11Device& device, const std::filesystem::path& path) {
    D3DPtr<ID3D11ShaderResourceView> texture;

    const HRESULT result = DirectX::CreateWICTextureFromFile(&device, path.c_str(), nullptr, &texture);
    CheckResult(result, [&] { return std::format("Failed to create texture from file '{}'", path.generic_string()); });

    return texture;
}

}  // namespace borov_engine::detail
