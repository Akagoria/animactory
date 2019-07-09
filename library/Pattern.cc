#include <animactory/Pattern.h>

#include <cassert>

namespace ay {

  namespace {

    cairo_extend_t translateTileMode(TileMode mode) {
      switch (mode) {
        case TileMode::Clamp:
          return CAIRO_EXTEND_PAD;
        case TileMode::Repeat:
          return CAIRO_EXTEND_REPEAT;
        case TileMode::Mirror:
          return CAIRO_EXTEND_REFLECT;
        case TileMode::Decal:
          return CAIRO_EXTEND_NONE;
      }

      assert(false);
      return CAIRO_EXTEND_NONE;
    }

    cairo_filter_t translateFilterQuality(FilterQuality filter) {
      switch (filter) {
        case FilterQuality::Fast:
          return CAIRO_FILTER_FAST;
        case FilterQuality::Good:
          return CAIRO_FILTER_GOOD;
        case FilterQuality::Best:
          return CAIRO_FILTER_BEST;
      }

      assert(false);
      return CAIRO_FILTER_GOOD;
    }

  }

  Pattern::~Pattern() {
    if (m_pattern != nullptr) {
      cairo_pattern_destroy(m_pattern);
    }
  }


  Pattern::Pattern(Pattern&& other) noexcept
  : m_pattern(std::exchange(other.m_pattern, nullptr))
  {

  }

  Pattern& Pattern::operator=(Pattern&& other) noexcept {
    std::swap(m_pattern, other.m_pattern);
    return *this;
  }


  void Pattern::addColorStop(double offset, gf::Color4d color) {
    cairo_pattern_add_color_stop_rgba(m_pattern, offset, color.r, color.g, color.b, color.a);
  }

  void Pattern::setTileMode(TileMode mode) {
    cairo_pattern_set_extend(m_pattern, translateTileMode(mode));
  }

  void Pattern::setFilterQuality(FilterQuality filter) {
    cairo_pattern_set_filter(m_pattern, translateFilterQuality(filter));
  }


  Pattern Pattern::makeSolid(gf::Color4d color) {
    return Pattern(cairo_pattern_create_rgba(color.r, color.g, color.b, color.a));
  }

  Pattern Pattern::makeLinear(gf::Vector2d p0, gf::Vector2d p1) {
    return Pattern(cairo_pattern_create_linear(p0.x, p0.y, p1.x, p1.y));
  }

  Pattern Pattern::makeRadial(gf::CircD c0, gf::CircD c1) {
    return Pattern(cairo_pattern_create_radial(c0.center.x, c0.center.y, c0.radius, c1.center.x, c1.center.y, c1.radius));
  }

  Pattern::Pattern(cairo_pattern_t *pattern)
  : m_pattern(pattern)
  {

  }


}

