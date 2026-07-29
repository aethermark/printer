#pragma once

#include "printer/detail/model.hpp"
#include "printer/reflect.hpp"
#include "printer/style.hpp"

namespace printer {

template <Reflectable T>
auto MakeTable(const T& object) -> Table {
  Table table;

  table.title = std::string(T::Name());

  table.columns = {
      {.header = "Field", .alignment = Alignment::kLeft, .is_major = true},
      {.header = "Value", .alignment = Alignment::kRight},
  };

  constexpr auto fields = T::Fields();

  std::apply(
      [&](const auto&... field) -> auto {
        (table.rows.push_back(Row{{
             Cell{.value = std::string(field.name)},
             Cell{field.get(object)},
         }}),
         ...);
      },
      fields);

  return table;
}

template <std::ranges::input_range Range>
  requires Reflectable<std::ranges::range_value_t<Range>>
auto MakeTable(const Range& range) -> Table {
  using T = std::ranges::range_value_t<Range>;

  Table table;

  table.title = std::string(T::Name()) + "s (" + std::to_string(std::ranges::distance(range)) + ")";

  constexpr auto fields = T::Fields();

  table.columns.push_back({.header = "#", .alignment = Alignment::kRight, .is_major = true});

  std::apply(
      [&](const auto&... field) -> auto {
        (table.columns.push_back(Column{
             .header = std::string(field.name),
             .alignment = field.alignment,
             .is_major = false,
         }),
         ...);
      },
      fields);

  size_t i = 0;
  for (const auto& object : range) {
    Row row;

    row.cells.push_back(Cell{.value = std::to_string(i++)});
    std::apply(
        [&](const auto&... field) -> auto { (row.cells.push_back({field.get(object)}), ...); },
        fields);

    table.rows.push_back(std::move(row));
  }

  return table;
}

}  // namespace printer
