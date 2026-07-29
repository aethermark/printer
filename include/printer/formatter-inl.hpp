#pragma once

#include "printer/detail/render.hpp"
#include "printer/formatter.hpp"

namespace printer {

template <Reflectable T>
[[nodiscard]]
auto Formatter::Render(const T& object) const -> std::string {
  return RenderImpl(MakeTable(object), state_);
}

template <std::ranges::input_range Range>
  requires Reflectable<std::ranges::range_value_t<Range>>
[[nodiscard]]
auto Formatter::Render(const Range& range) const -> std::string {
  return RenderImpl(MakeTable(range), state_);
}

template <Reflectable T>
auto Formatter::Print(std::ostream& os, const T& object) const -> std::ostream& {
  return os << Render(object);
}

template <std::ranges::input_range Range>
  requires Reflectable<std::ranges::range_value_t<Range>>
auto Formatter::Print(std::ostream& os, const Range& range) const -> std::ostream& {
  return os << Render(range);
}

}  // namespace printer
