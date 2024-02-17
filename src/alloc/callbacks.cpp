#include "borov_engine/alloc/callbacks.hpp"

#include <cstdlib>

namespace borov_engine::alloc {

namespace detail {

AllocateCallback allocate_callback = std::malloc;
FreeCallback free_callback = std::free;

}

void *Alloc(std::size_t size) {
    return detail::allocate_callback(size);
}

void Free(void *ptr) {
    detail::free_callback(ptr);
}

AllocateCallback GetAllocateCallback() {
    return detail::allocate_callback;
}

FreeCallback GetFreeCallback() {
    return detail::free_callback;
}

void SetAllocateCallback(AllocateCallback allocate_callback) {
    detail::allocate_callback = allocate_callback;
}

void SetFreeCallback(FreeCallback free_callback) {
    detail::free_callback = free_callback;
}

}
