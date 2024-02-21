#pragma once

#ifndef BOROV_ENGINE_D3D_PTR_HPP_INCLUDED
#define BOROV_ENGINE_D3D_PTR_HPP_INCLUDED

#include <wrl/client.h>

namespace borov_engine::detail {

template<typename T>
using D3DPtr = Microsoft::WRL::ComPtr<T>;

}

#endif //BOROV_ENGINE_D3D_PTR_HPP_INCLUDED
