#include "printer/formatter.hpp"

#include "printer/style.hpp"

namespace printer {

auto Formatter::Pretty() -> Formatter& {
  state_.layout = Layout::kPretty;
  return *this;
}

auto Formatter::Debug() -> Formatter& {
  state_.layout = Layout::kDebug;
  return *this;
}

auto Formatter::Unicode() -> Formatter& {
  state_.border = Border::kUnicode;
  return *this;
}

auto Formatter::ASCII() -> Formatter& {
  state_.border = Border::kASCII;
  return *this;
}

auto Formatter::Color() -> Formatter& {
  state_.colors = true;
  return *this;
}

auto Formatter::NoColor() -> Formatter& {
  state_.colors = false;
  return *this;
}

auto Formatter::HeaderColor(::printer::Color color) -> Formatter& {
  state_.header.color = color;
  return *this;
}

auto Formatter::MajorColor(::printer::Color color) -> Formatter& {
  state_.major.color = color;
  return *this;
}

auto Formatter::HeaderBold(bool value) -> Formatter& {
  state_.header.bold = value;
  return *this;
}

auto Formatter::MajorBold(bool value) -> Formatter& {
  state_.major.bold = value;
  return *this;
}

auto Formatter::HeaderAlignment(::printer::Alignment alignment) -> Formatter& {
  state_.header.alignment = alignment;
  return *this;
}

auto Formatter::MajorAlignment(::printer::Alignment alignment) -> Formatter& {
  state_.major.alignment = alignment;
  return *this;
}

auto Formatter::Alignment(::printer::Alignment alignment) -> Formatter& {
  state_.alignment = alignment;
  return *this;
}

[[nodiscard]] auto Formatter::GetState() const -> const FormatState& {
  return state_;
}

}  // namespace printer
