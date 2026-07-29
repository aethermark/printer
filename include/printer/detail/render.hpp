#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace printer {

enum class Alignment : unsigned char;
struct FormatState;
struct Table;
struct TextStyle;
enum class Border : unsigned char;
enum class Color : unsigned char;

struct Borders {
  std::string_view top_left;
  std::string_view top_mid;
  std::string_view top_right;

  std::string_view mid_left;
  std::string_view mid_mid;
  std::string_view mid_right;

  std::string_view bottom_left;
  std::string_view bottom_mid;
  std::string_view bottom_right;

  std::string_view horizontal;
  std::string_view vertical;
};

inline auto GetBorders(Border border) -> const Borders&;
inline auto Pad(std::string_view text, std::size_t width, Alignment align) -> std::string;
inline auto RenderDebug(const Table& table, [[maybe_unused]] const FormatState& fmt) -> std::string;
inline auto ColorCode(Color color) -> std::string_view;
inline auto ApplyStyle(std::string& out, const TextStyle& style, bool enabled) -> void;
// NOLINTBEGIN(bugprone-easily-swappable-parameters)
inline void DrawBorder(std::string& out, std::string_view left, std::string_view middle,
                       std::string_view right, std::string_view horizontal,
                       const std::vector<std::size_t>& widths);
// NOLINTEND(bugprone-easily-swappable-parameters)
inline auto RenderPretty(const Table& table, const FormatState& fmt) -> std::string;
auto RenderImpl(const Table& table, const FormatState& fmt) -> std::string;

}  // namespace printer
