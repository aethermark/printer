#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "printer/style.hpp"

namespace printer {

struct Cell {
  std::string value;
};

struct Column {
  std::string header;
  std::optional<Alignment> alignment;
  bool is_major = false;
};

struct Row {
  std::vector<Cell> cells;
};

struct Table {
  std::string title;

  std::vector<Column> columns;
  std::vector<Row> rows;

  bool show_title = true;
};

template <typename T>
auto ToCell(const T& value) -> std::string {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

inline auto ToCell(const std::string& value) -> std::string {
  return value;
}

inline auto ToCell(std::string_view value) -> std::string {
  return std::string(value);
}

inline auto ToCell(const char* value) -> std::string {
  return value;
}

template <typename T>
auto Hex(T value) -> std::string {
  std::ostringstream ss;
  ss << std::showbase << std::uppercase << std::hex << value;
  return ss.str();
}

}  // namespace printer
