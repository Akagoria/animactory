#ifndef AY_PAINT_H
#define AY_PAINT_H

#include <cstdint>

#include <gf/Color.h>
#include <gf/Vector.h>

#include "Pattern.h"

namespace ay {

  enum class Style : uint8_t {
    Stroke,
    Fill,
    StrokeAndFill,
  };

  enum class Cap : uint8_t {
    Butt,
    Round,
    Square,
  };

  enum class Join : uint8_t {
    Miter,
    Round,
    Bevel,
  };

  enum class AntiAlias : uint8_t {
    Fast,
    Good,
    Best,
  };

  enum class BlendOp : uint8_t {
    Clear,
    Src,
    Dst,
    SrcOver,
    DstOver,
    SrcIn,
    DstIn,
    SrcOut,
    DstOut,
    SrcATop,
    DstATop,
    Xor,
    Add,
    Multiply,
    Screen,
    Overlay,
    Darken,
    Lighten,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
  };

  class Paint {
  public:
    constexpr gf::Color4d getColor() const { return m_color; }
    constexpr void setColor(gf::Color4d color) { m_color = color; }

    constexpr double getAlpha() const { return m_color.a; }
    constexpr void setAlpha(double alpha) { m_color.a = alpha; }

    constexpr Style getStyle() const { return m_style; }
    constexpr void setStyle(Style style) { m_style = style; }

    constexpr Pattern* getPattern() const { return m_pattern; }
    constexpr void setPattern(Pattern* pattern) { m_pattern = pattern; }


    constexpr double getStrokeWidth() const { return m_width; }
    constexpr void setStrokeWidth(double width) { m_width = width; }

    constexpr double getStrokeMiter() const { return m_miter; }
    constexpr void setStrokeMiter(double miter) { m_miter = miter; }

    constexpr Cap getStrokeCap() const { return m_cap; }
    constexpr void setStrokeCap(Cap cap) { m_cap = cap; }

    constexpr Join getStrokeJoin() const { return m_join; }
    constexpr void setStrokeJoin(Join join) { m_join = join; }

    constexpr AntiAlias getAntiAlias() const { return m_antialias; }
    constexpr void setAntiAlias(AntiAlias antialias) { m_antialias = antialias; }

    constexpr BlendOp getBlendOp() const { return m_op; }
    constexpr void setBlendOp(BlendOp op) { m_op = op; }

  private:
    gf::Color4d m_color = gf::ColorD::Black;

    double m_width = 0.0; // stroke width
    double m_miter = 4.0;

    Pattern *m_pattern = nullptr;

    Style m_style = Style::Fill;
    Cap m_cap = Cap::Butt;
    Join m_join = Join::Miter;
    AntiAlias m_antialias = AntiAlias::Good;
    BlendOp m_op = BlendOp::SrcOver;
  };

}

#endif // AY_PAINT_H
