#pragma once

#ifndef BOROV_ENGINE_ALLOC_INLINE_ALLOCATOR_INL_INCLUDED
#define BOROV_ENGINE_ALLOC_INLINE_ALLOCATOR_INL_INCLUDED

#include <utility>
#include <cstring>

namespace borov_engine::alloc {

template<std::size_t MaxStackSize>
constexpr InlineAllocator<MaxStackSize>::InlineAllocator() noexcept: size_{}, alloc_{} {
    static_assert(MaxStackSize > sizeof(void *), "MaxStackSize is smaller or equal to the size of a pointer. "
                                                 "This will make the use of an InlineAllocator pointless. "
                                                 "Please increase the MaxStackSize.");
}

template<std::size_t MaxStackSize>
InlineAllocator<MaxStackSize>::~InlineAllocator() noexcept {
    Free();
}

template<std::size_t MaxStackSize>
InlineAllocator<MaxStackSize>::InlineAllocator(const InlineAllocator &other) : size_{}, alloc_{} {
    if (other.HasAllocation()) {
        std::memcpy(Allocate(other.size_), other.GetAllocation(), other.size_);
    }
    size_ = other.size_;
}

template<std::size_t MaxStackSize>
InlineAllocator<MaxStackSize> &InlineAllocator<MaxStackSize>::operator=(const InlineAllocator &other) {
    if (other.HasAllocation()) {
        std::memcpy(Allocate(other.size_), other.GetAllocation(), other.size_);
    }
    size_ = other.size_;
    return *this;
}

template<std::size_t MaxStackSize>
InlineAllocator<MaxStackSize>::InlineAllocator(InlineAllocator &&other) noexcept: size_{other.size_}, alloc_{} {
    other.size_ = 0;
    if (size_ > MaxStackSize) {
        std::swap(alloc_, other.alloc_);
    } else {
        std::memcpy(buffer_, other.buffer_, size_);
    }
}

template<std::size_t MaxStackSize>
InlineAllocator<MaxStackSize> &InlineAllocator<MaxStackSize>::operator=(InlineAllocator &&other) noexcept {
    Free();
    size_ = other.size_;
    other.size_ = 0;
    if (size_ > MaxStackSize) {
        std::swap(alloc_, other.alloc_);
    } else {
        std::memcpy(buffer_, other.buffer_, size_);
    }
    return *this;
}

template<std::size_t MaxStackSize>
void *InlineAllocator<MaxStackSize>::Allocate(const std::size_t size) {
    if (size_ != size) {
        Free();
        size_ = size;
        if (size > MaxStackSize) {
            alloc_ = std::malloc(size);
            return alloc_;
        }
    }
    return (void *) buffer_;
}

template<std::size_t MaxStackSize>
void InlineAllocator<MaxStackSize>::Free() {
    if (size_ > MaxStackSize) {
        std::free(alloc_);
    }
    size_ = 0;
}

template<std::size_t MaxStackSize>
constexpr void *InlineAllocator<MaxStackSize>::GetAllocation() const {
    if (HasAllocation()) {
        return HasHeapAllocation() ? alloc_ : (void *) buffer_;
    }
    return nullptr;
}

template<std::size_t MaxStackSize>
constexpr std::size_t InlineAllocator<MaxStackSize>::GetSize() const {
    return size_;
}

template<std::size_t MaxStackSize>
constexpr bool InlineAllocator<MaxStackSize>::HasAllocation() const {
    return size_ > 0;
}

template<std::size_t MaxStackSize>
constexpr bool InlineAllocator<MaxStackSize>::HasHeapAllocation() const {
    return size_ > MaxStackSize;
}

}

#endif //BOROV_ENGINE_ALLOC_INLINE_ALLOCATOR_INL_INCLUDED
