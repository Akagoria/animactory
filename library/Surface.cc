#include <animactory/Surface.h>

#include <cassert>

namespace ay {

  Surface::Surface(gf::Vector2i size)
  : Surface(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size.width, size.height))
  {

  }

  Surface::~Surface() {
    if (m_surface != nullptr) {
      cairo_surface_destroy(m_surface);
    }
  }

  Surface::Surface(Surface&& other) noexcept
  : m_surface(std::exchange(other.m_surface, nullptr))
  {

  }

  Surface& Surface::operator=(Surface&& other) noexcept {
    std::swap(m_surface, other.m_surface);
    return *this;
  }

  Surface Surface::subSurface(gf::RectI rect) {
    return Surface(cairo_surface_create_for_rectangle(m_surface, rect.left, rect.top, rect.width, rect.height));
  }

  void Surface::exportToPng(const gf::Path& path) {
    cairo_surface_write_to_png(m_surface, path.string().c_str());
  }

  Surface::Surface(cairo_surface_t *surface)
  : m_surface(surface)
  {
    assert(m_surface);
  }

}

