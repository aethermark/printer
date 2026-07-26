#pragma once

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

}  // namespace printer
