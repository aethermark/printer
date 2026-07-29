#pragma once

#include <iosfwd>
#include <ranges>
#include <string>

#include "printer/reflect.hpp"
#include "printer/style.hpp"

namespace printer {

class Formatter {
public:
  Formatter() = default;

  auto Pretty() -> Formatter&;
  auto Debug() -> Formatter&;
  auto Unicode() -> Formatter&;
  auto ASCII() -> Formatter&;
  auto Color() -> Formatter&;
  auto NoColor() -> Formatter&;
  auto HeaderColor(::printer::Color color) -> Formatter&;
  auto MajorColor(::printer::Color color) -> Formatter&;
  auto HeaderBold(bool value = true) -> Formatter&;
  auto MajorBold(bool value = true) -> Formatter&;
  auto HeaderAlignment(Alignment alignment) -> Formatter&;
  auto MajorAlignment(Alignment alignment) -> Formatter&;
  auto Alignment(Alignment alignment) -> Formatter&;
  auto ThemeDark() -> Formatter&;
  auto ThemeGruvbox() -> Formatter&;

  template <Reflectable T>
  [[nodiscard]] auto Render(const T& object) const -> std::string;
  template <std::ranges::input_range Range>
    requires Reflectable<std::ranges::range_value_t<Range>>
  [[nodiscard]]
  auto Render(const Range& range) const -> std::string;
  template <Reflectable T>
  auto Print(std::ostream& os, const T& object) const -> std::ostream&;
  template <std::ranges::input_range Range>
    requires Reflectable<std::ranges::range_value_t<Range>>
  auto Print(std::ostream& os, const Range& range) const -> std::ostream&;
  [[nodiscard]] auto GetState() const -> const FormatState&;

private:
  FormatState state_;
};

}  // namespace printer

#include "printer/formatter-inl.hpp"  // IWYU pragma: keep
