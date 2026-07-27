#pragma once

#include <optional>

#include "model.hpp"
#include "printer/style.hpp"

namespace printer {

template <typename Getter>
struct Field {
  std::string_view name;
  Getter get;
  std::optional<Alignment> alignment;
};

template <typename Getter>
constexpr auto MakeField(std::string_view name, Getter getter,
                         std::optional<Alignment> alignment = std::nullopt) {
  return Field<Getter>{.name = name, .get = std::move(getter), .alignment = alignment};
}

template <typename Class, typename Member>
constexpr auto MakeMemberField(std::string_view name, Member Class::* member,
                               std::optional<Alignment> alignment = std::nullopt) {
  return MakeField(
      name, [member](const Class& self) -> auto { return ToCell(self.*member); }, alignment);
}

template <typename T>
concept Reflectable = requires {
  T::Name();
  T::Fields();
};

}  // namespace printer
