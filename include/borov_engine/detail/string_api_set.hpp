#pragma once

#ifndef BOROV_ENGINE_DETAIL_STRING_API_SET_HPP_INCLUDED
#define BOROV_ENGINE_DETAIL_STRING_API_SET_HPP_INCLUDED

#include <windows.h>

#include <string>

namespace borov_engine::detail {

std::wstring MultiByteToWideChar(UINT code_page, DWORD dw_flags, std::string_view string);

}

#endif //BOROV_ENGINE_DETAIL_STRING_API_SET_HPP_INCLUDED
