#pragma once

#ifndef BOROV_ENGINE_CONCEPTS_HPP_INCLUDED
#define BOROV_ENGINE_CONCEPTS_HPP_INCLUDED

#include <ranges>

namespace borov_engine {

template <typename Range, typename T>
concept TypedRange = std::ranges::range<Range> && std::same_as<std::ranges::range_value_t<Range>, T>;

template <typename Range, typename T>
concept RefWrapperRange = TypedRange<Range, std::reference_wrapper<T>>;

}  // namespace borov_engine

#endif  // BOROV_ENGINE_CONCEPTS_HPP_INCLUDED
