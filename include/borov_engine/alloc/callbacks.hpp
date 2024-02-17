#pragma once

#ifndef BOROV_ENGINE_ALLOC_CALLBACKS_HPP_INCLUDED
#define BOROV_ENGINE_ALLOC_CALLBACKS_HPP_INCLUDED

#include <cstddef>

namespace borov_engine::alloc {

using AllocateCallback = void *(*)(std::size_t size);
using FreeCallback = void (*)(void *ptr);

namespace detail {

extern AllocateCallback allocate_callback;
extern FreeCallback free_callback;

}

void *Alloc(std::size_t size);
void Free(void *ptr);

AllocateCallback GetAllocateCallback();
FreeCallback GetFreeCallback();

void SetAllocateCallback(AllocateCallback allocate_callback);
void SetFreeCallback(FreeCallback free_callback);

}

#endif //BOROV_ENGINE_ALLOC_CALLBACKS_HPP_INCLUDED
