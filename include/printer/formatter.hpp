#pragma once

#include <ranges>

#include "printer/reflect.hpp"
#include "printer/style.hpp"

namespace printer {

class Formatter {
public:
  Formatter() = default;

  auto Pretty() -> Formatter& {
    state_.layout = Layout::kPretty;
    return *this;
  }

  auto Debug() -> Formatter& {
    state_.layout = Layout::kDebug;
    return *this;
  }

  auto Unicode() -> Formatter& {
    state_.border = Border::kUnicode;
    return *this;
  }

  auto ASCII() -> Formatter& {
    state_.border = Border::kASCII;
    return *this;
  }

  auto Color() -> Formatter& {
    state_.colors = true;
    return *this;
  }

  auto NoColor() -> Formatter& {
    state_.colors = false;
    return *this;
  }

  auto HeaderColor(enum Color color) -> Formatter& {
    state_.header.color = color;
    return *this;
  }

  auto MajorColor(enum Color color) -> Formatter& {
    state_.major.color = color;
    return *this;
  }

  auto HeaderBold(bool value = true) -> Formatter& {
    state_.header.bold = value;
    return *this;
  }

  auto MajorBold(bool value = true) -> Formatter& {
    state_.major.bold = value;
    return *this;
  }

  auto HeaderAlignment(Alignment alignment) -> Formatter& {
    state_.header.alignment = alignment;
    return *this;
  }

  auto MajorAlignment(Alignment alignment) -> Formatter& {
    state_.major.alignment = alignment;
    return *this;
  }

  auto Alignment(Alignment alignment) -> Formatter& {
    state_.alignment = alignment;
    return *this;
  }

  template <Reflectable T>
  [[nodiscard]]
  auto Render(const T& object) const -> std::string {
    return Render(MakeTable(object), state_);
  }

  template <std::ranges::input_range Range>
    requires Reflectable<std::ranges::range_value_t<Range>>
  [[nodiscard]]
  auto Render(const Range& range) const -> std::string {
    return Render(MakeTable(range), state_);
  }

  template <Reflectable T>
  auto Print(std::ostream& os, const T& object) const -> std::ostream& {
    return os << Render(object);
  }

  template <std::ranges::input_range Range>
    requires Reflectable<std::ranges::range_value_t<Range>>
  auto Print(std::ostream& os, const Range& range) const -> std::ostream& {
    return os << Render(range);
  }

  [[nodiscard]] auto State() const -> const FormatState& {
    return state_;
  }

private:
  FormatState state_;
};

}  // namespace printer
