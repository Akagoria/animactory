#ifndef AY_SURFACE_H
#define AY_SURFACE_H

#include <cairo.h>

#include <gf/Path.h>
#include <gf/Rect.h>
#include <gf/Vector.h>

namespace ay {

  class Surface {
  public:
    Surface(gf::Vector2i size);

    ~Surface();

    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    Surface(Surface&& other) noexcept;
    Surface& operator=(Surface&& other) noexcept;

    Surface subSurface(gf::RectI rect);
    void exportToPng(const gf::Path& path);

  private:
    friend class Canvas;
    Surface(cairo_surface_t *surface);

    cairo_surface_t *m_surface;
  };


}


#endif // AY_SURFACE_H
