#pragma once

#ifndef BOROV_ENGINE_DETAIL_ERASED_UNIQUE_PTR_HPP_INCLUDED
#define BOROV_ENGINE_DETAIL_ERASED_UNIQUE_PTR_HPP_INCLUDED

#include <memory>

namespace borov_engine::detail {

using erased_delete = void (*)(void *);

constexpr erased_delete noop_erased_delete = [](void *) {};

template <typename T>
requires(!std::is_array_v<T>)
constexpr erased_delete single_erased_delete = [](void *erased_ptr) {
    constexpr std::default_delete<T> default_delete;
    auto ptr = static_cast<T *>(erased_ptr);
    default_delete(ptr);
};

template <typename T>
requires std::is_array_v<T>
constexpr erased_delete array_erased_delete = [](void *erased_ptr) {
    constexpr std::default_delete<T> default_delete;
    auto ptr = static_cast<std::remove_extent_t<T> *>(erased_ptr);
    default_delete(ptr);
};

template <typename T>
requires(!std::is_array_v<T> && !std::is_void_v<T>)
constexpr erased_delete default_erased_delete() {
    return single_erased_delete<T>;
}

template <typename T = void>
constexpr erased_delete default_erased_delete() {
    return noop_erased_delete;
}

template <typename T>
requires std::is_array_v<T>
constexpr erased_delete default_erased_delete() {
    return array_erased_delete<T>;
}

template <typename T>
requires std::is_bounded_array_v<T>
constexpr erased_delete default_erased_delete() = delete;

using erased_unique_ptr = std::unique_ptr<void, erased_delete>;

template <typename T>
requires(!std::is_array_v<T>)
erased_unique_ptr erased_unique(T *ptr) {
    return {ptr, single_erased_delete<T>};
}

template <typename T>
requires std::is_array_v<T>
erased_unique_ptr erased_unique(std::remove_extent_t<T> *ptr) {
    return {ptr, array_erased_delete<T>};
}

template <typename T>
requires std::is_bounded_array_v<T>
erased_unique_ptr erased_unique(std::remove_extent_t<T> *) = delete;

template <>
inline erased_unique_ptr erased_unique(void *ptr) {
    return {ptr, noop_erased_delete};
}

inline erased_unique_ptr erased_unique() noexcept {
    return {nullptr, noop_erased_delete};
}

inline erased_unique_ptr erased_unique(std::nullptr_t) {
    return erased_unique();
}

template <typename T, typename... Args>
requires(!std::is_array_v<T>)
erased_unique_ptr make_erased_unique(Args &&...args) {
    return {new T{std::forward<Args>(args)...}, single_erased_delete<T>};
}

template <typename T>
requires std::is_array_v<T>
erased_unique_ptr make_erased_unique(std::size_t size) {
    return {new std::remove_extent_t<T>[size] {}, array_erased_delete<T>};
}

template <typename T, typename... Args>
requires std::is_bounded_array_v<T>
erased_unique_ptr make_erased_unique(Args &&...) = delete;

template <typename T>
requires(!std::is_array_v<T>)
erased_unique_ptr make_erased_unique_for_overwrite() {
    return {new T, single_erased_delete<T>};
}

template <typename T>
requires std::is_array_v<T>
erased_unique_ptr make_erased_unique_for_overwrite(std::size_t size) {
    return {new std::remove_extent_t<T>[size], array_erased_delete<T>};
}

template <typename T, typename... Args>
requires std::is_bounded_array_v<T>
erased_unique_ptr make_erased_unique_for_overwrite(Args &&...) = delete;

template <typename T>
requires(!std::is_array_v<T>)
erased_unique_ptr erase(std::unique_ptr<T> ptr) {
    return {ptr.release(), single_erased_delete<T>};
}

template <typename T>
requires std::is_array_v<T>
erased_unique_ptr erase(std::unique_ptr<T> ptr) {
    return {ptr.release(), array_erased_delete<T>};
}

}  // namespace borov_engine::detail

#endif  // BOROV_ENGINE_DETAIL_ERASED_UNIQUE_PTR_HPP_INCLUDED
