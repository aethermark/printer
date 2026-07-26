#pragma once

#include <algorithm>
#include <cstring>
#include <ios>
#include <iterator>
#include <optional>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace printer {

enum class Layout : unsigned char {
  kDebug,
  kPretty,
};

enum class Border : unsigned char { kASCII, kUnicode };

enum class Color : unsigned char {
  kDefault,
  kBlack,
  kRed,
  kGreen,
  kYellow,
  kBlue,
  kMagenta,
  kCyan,
  kWhite
};

enum class Alignment : unsigned char {
  kLeft,
  kCenter,
  kRight,
};

struct TextStyle {
  Color color = Color::kDefault;
  bool bold = false;
  Alignment alignment = Alignment::kLeft;
};

struct FormatState {
  Layout layout = Layout::kDebug;
  Border border = Border::kUnicode;
  bool colors = true;

  TextStyle header{
      .color = Color::kCyan,
      .bold = true,
  };

  TextStyle major{
      .color = Color::kGreen,
      .bold = false,
  };

  Alignment alignment = Alignment::kLeft;
};

inline auto FormatIndex() -> int {
  static int idx = std::ios_base::xalloc();
  return idx;
}

inline auto State(std::ostream &os) -> FormatState & {
  auto *&ptr = reinterpret_cast<FormatState *&>(os.pword(FormatIndex()));

  if (ptr == nullptr) {
    ptr = new FormatState;
    os.register_callback(
        [](std::ios_base::event ev, std::ios_base &stream, int index) -> void {
          if (ev == std::ios_base::erase_event) {
            delete static_cast<FormatState *>(stream.pword(index));
            stream.pword(index) = nullptr;
          }
        },
        FormatIndex());
  }

  return *ptr;
}

inline auto pretty(std::ostream &os) -> std::ostream & {
  State(os).layout = Layout::kPretty;
  return os;
}

inline auto debug(std::ostream &os) -> std::ostream & {
  State(os).layout = Layout::kDebug;
  return os;
}

inline auto unicode(std::ostream &os) -> std::ostream & {
  State(os).border = Border::kUnicode;
  return os;
}

inline auto ascii(std::ostream &os) -> std::ostream & {
  State(os).border = Border::kASCII;
  return os;
}

inline auto color(std::ostream &os) -> std::ostream & {
  State(os).colors = true;
  return os;
}

inline auto no_color(std::ostream &os) -> std::ostream & {
  State(os).colors = false;
  return os;
}

inline auto header_bold(std::ostream &os) -> std::ostream & {
  State(os).header.bold = true;
  return os;
}

inline auto header_normal(std::ostream &os) -> std::ostream & {
  State(os).header.bold = false;
  return os;
}

inline auto major_bold(std::ostream &os) -> std::ostream & {
  State(os).major.bold = true;
  return os;
}

inline auto major_normal(std::ostream &os) -> std::ostream & {
  State(os).major.bold = false;
  return os;
}

class HeaderColor {
public:
  explicit HeaderColor(Color c) : color_(c) {}

  friend auto operator<<(std::ostream &os, const HeaderColor &h)
      -> std::ostream & {
    State(os).header.color = h.color_;
    return os;
  }

private:
  Color color_;
};

inline auto header(Color c) -> HeaderColor { return HeaderColor(c); }

class MajorColor {
public:
  explicit MajorColor(Color c) : color_(c) {}

  friend auto operator<<(std::ostream &os, const MajorColor &m)
      -> std::ostream & {
    State(os).major.color = m.color_;
    return os;
  }

private:
  Color color_;
};

inline auto major(Color c) -> MajorColor { return MajorColor(c); }

class HeaderAlignment {
public:
  explicit HeaderAlignment(Alignment a) : alignment_(a) {}

  friend auto operator<<(std::ostream &os, const HeaderAlignment &h)
      -> std::ostream & {
    State(os).header.alignment = h.alignment_;
    return os;
  }

private:
  Alignment alignment_;
};

inline auto header_align(Alignment a) { return HeaderAlignment(a); }

class MajorAlignment {
public:
  explicit MajorAlignment(Alignment a) : alignment_(a) {}

  friend auto operator<<(std::ostream &os, const MajorAlignment &m)
      -> std::ostream & {
    State(os).major.alignment = m.alignment_;
    return os;
  }

private:
  Alignment alignment_;
};

inline auto major_align(Alignment a) { return MajorAlignment(a); }

class DataAlignment {
public:
  explicit DataAlignment(Alignment a) : alignment_(a) {}

  friend auto operator<<(std::ostream &os, const DataAlignment &d)
      -> std::ostream & {
    State(os).alignment = d.alignment_;
    return os;
  }

private:
  Alignment alignment_;
};

inline auto align(Alignment a) { return DataAlignment(a); }

inline auto left(std::ostream &os) -> std::ostream & {
  State(os).alignment = Alignment::kLeft;
  return os;
}

inline auto center(std::ostream &os) -> std::ostream & {
  State(os).alignment = Alignment::kCenter;
  return os;
}

inline auto right(std::ostream &os) -> std::ostream & {
  State(os).alignment = Alignment::kRight;
  return os;
}

namespace theme {

class Dark {
public:
  friend auto operator<<(std::ostream &os, Dark /*unused*/) -> std::ostream & {
    auto &s = State(os);

    s.layout = Layout::kPretty;
    s.border = Border::kUnicode;
    s.colors = true;
    s.header.color = Color::kCyan;
    s.header.bold = true;
    s.major.color = Color::kCyan;
    s.major.bold = true;

    return os;
  }
};

inline constexpr Dark dark{};
} // namespace theme

namespace printer::ascii {

inline constexpr auto kReset = "\033[0m";
inline constexpr auto kBold = "\033[1m";

inline constexpr auto kBlue = "\033[34m";
inline constexpr auto kCyan = "\033[36m";
inline constexpr auto kGreen = "\033[32m";
inline constexpr auto kYellow = "\033[33m";
inline constexpr auto kGray = "\033[90m";

} // namespace printer::ascii

template <typename T> auto ToCell(const T &value) -> std::string {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

inline auto ToCell(const std::string &value) -> std::string { return value; }

inline auto ToCell(std::string_view value) -> std::string {
  return std::string(value);
}

inline auto ToCell(const char *value) -> std::string { return value; }

template <typename T> auto Hex(T value) -> std::string {
  std::ostringstream ss;
  ss << std::showbase << std::uppercase << std::hex << value;
  return ss.str();
}

template <typename Getter> struct Field {
  std::string_view name;
  Getter get;
  std::optional<Alignment> alignment;
};

template <typename Getter>
constexpr auto MakeField(std::string_view name, Getter getter,
                         std::optional<Alignment> alignment = std::nullopt) {
  return Field<Getter>{
      .name = name, .get = std::move(getter), .alignment = alignment};
}

template <typename Class, typename Member>
constexpr auto
MakeMemberField(std::string_view name, Member Class::*member,
                std::optional<Alignment> alignment = std::nullopt) {
  return MakeField(
      name,
      [member](const Class &self) -> auto { return ToCell(self.*member); },
      alignment);
}

template <typename T>
concept Reflectable = requires {
  T::Name();
  T::Fields();
};

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

template <Reflectable T> auto MakeTable(const T &object) -> Table {
  Table table;

  table.title = std::string(T::Name());

  table.columns = {
      {.header = "Field", .alignment = Alignment::kLeft, .is_major = true},
      {.header = "Value", .alignment = Alignment::kRight},
  };

  constexpr auto fields = T::Fields();

  std::apply(
      [&](const auto &...field) -> auto {
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
auto MakeTable(const Range &range) -> Table {
  using T = std::ranges::range_value_t<Range>;

  Table table;

  table.title = std::string(T::Name()) + "s (" +
                std::to_string(std::ranges::distance(range)) + ")";

  constexpr auto fields = T::Fields();

  table.columns.push_back(
      {.header = "#", .alignment = Alignment::kRight, .is_major = true});

  std::apply(
      [&](const auto &...field) -> auto {
        (table.columns.push_back(Column{
             .header = std::string(field.name),
             .alignment = field.alignment,
             .is_major = false,
         }),
         ...);
      },
      fields);

  int i = 0;
  for (const auto &object : range) {
    Row row;

    row.cells.push_back(Cell{.value = std::to_string(i++)});
    std::apply(
        [&](const auto &...field) -> auto {
          (row.cells.push_back({field.get(object)}), ...);
        },
        fields);

    table.rows.push_back(std::move(row));
  }

  return table;
}

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

inline auto GetBorders(Border border) -> const Borders & {
  return border == Border::kUnicode ? kUnicode : kASCII;
}

inline auto BeginStyle(std::ostream &os, const TextStyle &style, bool enabled)
    -> std::ostream & {
  if (!enabled) {
    return os;
  }

  if (style.bold) {
    os << "\033[1m";
  }

  switch (style.color) {
  case Color::kRed:
    os << "\033[31m";
    break;
  case Color::kGreen:
    os << "\033[32m";
    break;
  case Color::kYellow:
    os << "\033[33m";
    break;
  case Color::kBlue:
    os << "\033[34m";
    break;
  case Color::kMagenta:
    os << "\033[35m";
    break;
  case Color::kCyan:
    os << "\033[36m";
    break;
  case Color::kWhite:
    os << "\033[37m";
    break;
  default:
    break;
  }

  return os;
}

inline auto EndStyle(std::ostream &os, bool enabled) -> std::ostream & {
  if (enabled) {
    os << "\033[0m";
  }

  return os;
}

inline auto Pad(std::string_view text, std::size_t width, Alignment align)
    -> std::string {
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

inline auto RenderDebug(std::ostream &os, const Table &table)
    -> std::ostream & {
  os << table.title << " {\n";

  if (table.columns.size() == 2 && table.columns[0].header == "Field") {
    for (auto const &row : table.rows) {
      os << "  " << row.cells[0].value << ": " << row.cells[1].value << '\n';
    }
  } else {
    for (auto const &row : table.rows) {
      os << "  ";

      for (std::size_t i = 0; i < row.cells.size(); ++i) {
        os << table.columns[i].header << '=' << row.cells[i].value;

        if (i + 1 != row.cells.size()) {
          os << ", ";
        }
      }

      os << '\n';
    }
  }

  return os << '}';
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

inline auto ApplyStyle(std::ostream &os, const TextStyle &style, bool enabled)
    -> void {
  if (!enabled) {
    return;
  }

  if (style.bold) {
    os << "\033[1m";
  }

  os << ColorCode(style.color);
}

inline auto ResetStyle(std::ostream &os, bool enabled) -> void {
  if (enabled) {
    os << "\033[0m";
  }
}

inline void Repeat(std::ostream &os, std::string_view text, std::size_t count) {
  while (count-- > 0) {
    os << text;
  }
}

inline void DrawBorder(std::ostream &os, std::string_view left,
                       std::string_view middle, std::string_view right,
                       std::string_view horizontal,
                       const std::vector<std::size_t> &widths) {
  os << left;

  for (std::size_t i = 0; i < widths.size(); ++i) {
    Repeat(os, horizontal, widths[i] + 2);

    if (i + 1 != widths.size()) {
      os << middle;
    }
  }

  os << right << '\n';
}

inline auto RenderPretty(std::ostream &os, const Table &table)
    -> std::ostream & {
  auto const &fmt = State(os);
  auto const &border = GetBorders(fmt.border);

  std::vector<std::size_t> widths(table.columns.size());

  for (std::size_t i = 0; i < table.columns.size(); ++i) {
    widths[i] = table.columns[i].header.size();
  }

  for (auto const &row : table.rows) {
    for (std::size_t i = 0; i < row.cells.size(); ++i) {
      widths[i] = std::max(widths[i], row.cells[i].value.size());
    }
  }

  if (table.show_title) {
    ApplyStyle(os, fmt.header, fmt.colors);
    os << table.title;
    ResetStyle(os, fmt.colors);
    os << "\n";
  }

  DrawBorder(os, border.top_left, border.top_mid, border.top_right,
             border.horizontal, widths);

  os << border.vertical;

  for (std::size_t i = 0; i < table.columns.size(); ++i) {
    ApplyStyle(os, fmt.header, fmt.colors);

    os << ' ' << Pad(table.columns[i].header, widths[i], fmt.header.alignment)
       << ' ';

    ResetStyle(os, fmt.colors);

    os << border.vertical;
  }

  os << '\n';

  DrawBorder(os, border.mid_left, border.mid_mid, border.mid_right,
             border.horizontal, widths);

  for (auto const &row : table.rows) {
    os << border.vertical;

    for (std::size_t i = 0; i < row.cells.size(); ++i) {
      Alignment alignment = table.columns[i].alignment.value_or(fmt.alignment);

      const TextStyle *style = nullptr;

      if (table.columns[0].is_major && i == 0) {
        style = &fmt.major;
        alignment = table.columns[i].alignment.value_or(fmt.major.alignment);
      }

      if (style != nullptr) {
        ApplyStyle(os, *style, fmt.colors);
      }

      os << ' ' << Pad(row.cells[i].value, widths[i], alignment) << ' ';

      if (style != nullptr) {
        ResetStyle(os, fmt.colors);
      }

      os << border.vertical;
    }

    os << '\n';
  }

  DrawBorder(os, border.bottom_left, border.bottom_mid, border.bottom_right,
             border.horizontal, widths);

  return os;
}

inline auto Render(std::ostream &os, const Table &table) -> std::ostream & {
  switch (State(os).layout) {
  case Layout::kDebug:
    return RenderDebug(os, table);

  case Layout::kPretty:
    return RenderPretty(os, table);
  }

  return os;
}

template <Reflectable T>
auto operator<<(std::ostream &os, const T &object) -> std::ostream & {
  return Render(os, MakeTable(object));
}

template <std::ranges::input_range Range>
  requires Reflectable<std::ranges::range_value_t<Range>>
auto operator<<(std::ostream &os, const Range &range) -> std::ostream & {
  return Render(os, MakeTable(range));
}

} // namespace printer
