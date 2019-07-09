#ifndef AY_PATTERN_H
#define AY_PATTERN_H

#include <type_traits>

#include <cairo.h>

#include <gf/Circ.h>
#include <gf/Vector.h>

namespace ay {

  enum class TileMode {
    Clamp,
    Repeat,
    Mirror,
    Decal,
  };

  enum class FilterQuality {
    Fast,
    Good,
    Best,
  };

  class Pattern {
  public:

    ~Pattern();

    Pattern(const Pattern&) = delete;
    Pattern& operator=(const Pattern&) = delete;

    Pattern(Pattern&& other) noexcept;
    Pattern& operator=(Pattern&& other) noexcept;

    void addColorStop(double offset, gf::Color4d color);
    void setTileMode(TileMode mode);
    void setFilterQuality(FilterQuality filter);

    static Pattern makeSolid(gf::Color4d color);
    static Pattern makeLinear(gf::Vector2d p0, gf::Vector2d p1);
    static Pattern makeRadial(gf::CircD c0, gf::CircD c1);

  private:
    Pattern(cairo_pattern_t *pattern);

    friend class Canvas;
    cairo_pattern_t *m_pattern;
  };

}

#endif // AY_PATTERN_H
