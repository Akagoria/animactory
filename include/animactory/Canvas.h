#ifndef AY_CANVAS_H
#define AY_CANVAS_H

#include <cairo.h>

#include <gf/Path.h>
#include <gf/Vector.h>

#include "Paint.h"
#include "Path.h"
#include "Surface.h"

namespace ay {

  enum class ClipOp {
    Difference,
    Intersect,
  };

  class Canvas {
  public:
    Canvas(Surface& surface);
    ~Canvas();

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    Canvas(Canvas&& other) noexcept;
    Canvas& operator=(Canvas&& other) noexcept;

    void save();
    void restore();

    void translate(gf::Vector2d t);
    void scale(gf::Vector2d s);
    void rotate(double angle);

    void clipRect(gf::RectD rect, ClipOp op = ClipOp::Intersect);
    void clipCircle(gf::CircD circ, ClipOp op = ClipOp::Intersect);
    void clipPath(const Path& path, ClipOp op = ClipOp::Intersect);

//     void drawColor(Color4d color, BlendOp op = BlendOp::SrcOver);

//     void clear(Color4d color) {
//       drawColor(color, BlendOp::Src);
//     }

    void drawPaint(const Paint& paint);

    // drawPoints
    // drawPoint

    void drawLine(gf::Vector2d p0, gf::Vector2d p1, const Paint& paint);
    void drawRect(gf::RectD rect, const Paint& paint);
    void drawCircle(gf::CircD circ, const Paint& paint);
    void drawArc(gf::CircD circ, double angle0, double angle1, const Paint& paint);
//     void drawRoundedRect(gf::RectD rect, gf::Vector2d radius, const Paint& paint);
    void drawPath(const Path& path, const Paint& paint);

  private:
    void setContextFromPaint(const Paint& paint);
    void setPath(const Path& path);
    void applyStyle(const Paint& paint);

    cairo_t *m_context;
  };


  class CanvasGuard {
  public:
    CanvasGuard(Canvas& canvas)
    : m_canvas(canvas)
    {
      m_canvas.save();
    }

    ~CanvasGuard() {
      m_canvas.restore();
    }

  private:
    Canvas& m_canvas;
  };

}

#endif // AY_CANVAS_H
