#pragma once

#include <algorithm>
#include <string>
#include <string_view>
#include <utility>

#include "printer/model.hpp"

namespace printer {

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

inline constexpr Borders kUnicode{.top_left = "┌",
                                  .top_mid = "┬",
                                  .top_right = "┐",
                                  .mid_left = "├",
                                  .mid_mid = "┼",
                                  .mid_right = "┤",
                                  .bottom_left = "└",
                                  .bottom_mid = "┴",
                                  .bottom_right = "┘",
                                  .horizontal = "─",
                                  .vertical = "│"};

inline constexpr Borders kASCII{.top_left = "+",
                                .top_mid = "+",
                                .top_right = "+",
                                .mid_left = "+",
                                .mid_mid = "+",
                                .mid_right = "+",
                                .bottom_left = "+",
                                .bottom_mid = "+",
                                .bottom_right = "+",
                                .horizontal = "-",
                                .vertical = "|"};

inline auto GetBorders(Border border) -> const Borders& {
  return border == Border::kUnicode ? kUnicode : kASCII;
}

inline auto Pad(std::string_view text, std::size_t width, Alignment align) -> std::string {
  if (text.size() >= width) {
    return std::string(text);
  }

  auto extra = width - text.size();

  switch (align) {
    case Alignment::kLeft:
      return std::string(text) + std::string(extra, ' ');

    case Alignment::kRight:
      return std::string(extra, ' ') + std::string(text);

    case Alignment::kCenter: {
      auto left = extra / 2;
      auto right = extra - left;

      return std::string(left, ' ') + std::string(text) + std::string(right, ' ');
    }
  }

  return std::string(text);
}

inline auto RenderDebug(const Table& table, [[maybe_unused]] const FormatState& fmt)
    -> std::string {
  std::string out;
  out += table.title;
  out += " {\n";

  if (table.columns.size() == 2 && table.columns.at(0).header == "Field") {
    for (auto const& row : table.rows) {
      out += "  ";
      out += row.cells.at(0).value;
      out += ": ";
      out += row.cells.at(1).value;
      out += '\n';
    }
  } else {
    for (auto const& row : table.rows) {
      out += "  ";

      for (std::size_t i = 0; i < row.cells.size(); ++i) {
        out += table.columns.at(i).header;
        out += '=';
        out += row.cells.at(i).value;

        if (i + 1 != row.cells.size()) {
          out += ", ";
        }
      }

      out += '\n';
    }
  }

  out += '}';
  return out;
}

inline auto ColorCode(Color color) -> std::string_view {
  switch (color) {
    case Color::kBlack:
      return "\033[30m";
    case Color::kRed:
      return "\033[31m";
    case Color::kGreen:
      return "\033[32m";
    case Color::kYellow:
      return "\033[33m";
    case Color::kBlue:
      return "\033[34m";
    case Color::kMagenta:
      return "\033[35m";
    case Color::kCyan:
      return "\033[36m";
    case Color::kWhite:
      return "\033[37m";
    default:
      return "";
  }
}

inline auto ApplyStyle(std::string& out, const TextStyle& style, bool enabled) -> void {
  if (!enabled) {
    return;
  }

  if (style.bold) {
    out += "\033[1m";
  }

  out += ColorCode(style.color);
}

inline auto ResetStyle(std::string& out, bool enabled) -> void {
  if (enabled) {
    out += "\033[0m";
  }
}

inline void Repeat(std::string& out, std::string_view text, std::size_t count) {
  while (count-- > 0) {
    out += text;
  }
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
inline void DrawBorder(std::string& out, std::string_view left, std::string_view middle,
                       std::string_view right, std::string_view horizontal,
                       const std::vector<std::size_t>& widths) {
  // NOLINTEND(bugprone-easily-swappable-parameters)

  out += left;

  for (std::size_t i = 0; i < widths.size(); ++i) {
    Repeat(out, horizontal, widths.at(i) + 2);

    if (i + 1 != widths.size()) {
      out += middle;
    }
  }

  out += right;
  out += "\n";
}

inline auto RenderPretty(const Table& table, const FormatState& fmt) -> std::string {
  std::string out;
  auto const& border = GetBorders(fmt.border);

  std::vector<std::size_t> widths(table.columns.size());

  for (std::size_t i = 0; i < table.columns.size(); ++i) {
    widths.at(i) = table.columns.at(i).header.size();
  }

  for (auto const& row : table.rows) {
    for (std::size_t i = 0; i < row.cells.size(); ++i) {
      widths.at(i) = std::max(widths.at(i), row.cells.at(i).value.size());
    }
  }

  if (table.show_title) {
    ApplyStyle(out, fmt.header, fmt.colors);
    out += table.title;
    ResetStyle(out, fmt.colors);
    out += "\n";
  }

  DrawBorder(out, border.top_left, border.top_mid, border.top_right, border.horizontal, widths);

  out += border.vertical;

  for (std::size_t i = 0; i < table.columns.size(); ++i) {
    ApplyStyle(out, fmt.header, fmt.colors);

    out += ' ';
    out += Pad(table.columns.at(i).header, widths.at(i), fmt.header.alignment);
    out += ' ';

    ResetStyle(out, fmt.colors);

    out += border.vertical;
  }

  out += '\n';

  DrawBorder(out, border.mid_left, border.mid_mid, border.mid_right, border.horizontal, widths);

  for (auto const& row : table.rows) {
    out += border.vertical;

    for (std::size_t i = 0; i < row.cells.size(); ++i) {
      Alignment alignment = table.columns.at(i).alignment.value_or(fmt.alignment);

      const TextStyle* style = nullptr;

      if (table.columns.at(0).is_major && i == 0) {
        style = &fmt.major;
        alignment = table.columns.at(i).alignment.value_or(fmt.major.alignment);
      }

      if (style != nullptr) {
        ApplyStyle(out, *style, fmt.colors);
      }

      out += ' ';
      out += Pad(row.cells.at(i).value, widths.at(i), alignment);
      out += ' ';

      if (style != nullptr) {
        ResetStyle(out, fmt.colors);
      }

      out += border.vertical;
    }

    out += '\n';
  }

  DrawBorder(out, border.bottom_left, border.bottom_mid, border.bottom_right, border.horizontal,
             widths);

  return out;
}

inline auto RenderImpl(const Table& table, const FormatState& fmt) -> std::string {
  switch (fmt.layout) {
    case Layout::kDebug:
      return RenderDebug(table, fmt);

    case Layout::kPretty:
      return RenderPretty(table, fmt);
  }
  std::unreachable();
}

}  // namespace printer
