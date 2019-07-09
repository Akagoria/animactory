#include <animactory/Canvas.h>

#include <cassert>

namespace ay {

  namespace {

    cairo_fill_rule_t translateClipOp(ClipOp op) {
      switch (op) {
        case ClipOp::Difference:
          return CAIRO_FILL_RULE_EVEN_ODD;
        case ClipOp::Intersect:
          return CAIRO_FILL_RULE_WINDING;
      }

      assert(false);
      return CAIRO_FILL_RULE_WINDING;
    }

    cairo_line_cap_t translateCap(Cap cap) {
      switch (cap) {
        case Cap::Butt:
          return CAIRO_LINE_CAP_BUTT;
        case Cap::Round:
          return CAIRO_LINE_CAP_ROUND;
        case Cap::Square:
          return CAIRO_LINE_CAP_SQUARE;
      }

      assert(false);
      return CAIRO_LINE_CAP_BUTT;
    }

    cairo_line_join_t translateJoin(Join join) {
      switch (join) {
        case Join::Miter:
          return CAIRO_LINE_JOIN_MITER;
        case Join::Round:
          return CAIRO_LINE_JOIN_ROUND;
        case Join::Bevel:
          return CAIRO_LINE_JOIN_BEVEL;
      }

      assert(false);
      return CAIRO_LINE_JOIN_MITER;
    }

    cairo_antialias_t translateAntiAlias(AntiAlias antialias) {
      switch (antialias) {
        case AntiAlias::Fast:
          return CAIRO_ANTIALIAS_FAST;
        case AntiAlias::Good:
          return CAIRO_ANTIALIAS_GOOD;
        case AntiAlias::Best:
          return CAIRO_ANTIALIAS_BEST;
      }

      assert(false);
      return CAIRO_ANTIALIAS_GOOD;
    }

    cairo_operator_t translateBlendOp(BlendOp op) {
      switch (op) {
        case BlendOp::Clear:
          return CAIRO_OPERATOR_CLEAR;
        case BlendOp::Src:
          return CAIRO_OPERATOR_SOURCE;
        case BlendOp::Dst:
          return CAIRO_OPERATOR_DEST;
        case BlendOp::SrcOver:
          return CAIRO_OPERATOR_OVER;
        case BlendOp::DstOver:
          return CAIRO_OPERATOR_DEST_OVER;
        case BlendOp::SrcIn:
          return CAIRO_OPERATOR_IN;
        case BlendOp::DstIn:
          return CAIRO_OPERATOR_DEST_IN;
        case BlendOp::SrcOut:
          return CAIRO_OPERATOR_OUT;
        case BlendOp::DstOut:
          return CAIRO_OPERATOR_DEST_OUT;
        case BlendOp::SrcATop:
          return CAIRO_OPERATOR_ATOP;
        case BlendOp::DstATop:
          return CAIRO_OPERATOR_DEST_ATOP;
        case BlendOp::Xor:
          return CAIRO_OPERATOR_XOR;
        case BlendOp::Add:
          return CAIRO_OPERATOR_ADD;
        case BlendOp::Multiply:
          return CAIRO_OPERATOR_MULTIPLY;
        case BlendOp::Screen:
          return CAIRO_OPERATOR_SCREEN;
        case BlendOp::Overlay:
          return CAIRO_OPERATOR_OVERLAY;
        case BlendOp::Darken:
          return CAIRO_OPERATOR_DARKEN;
        case BlendOp::Lighten:
          return CAIRO_OPERATOR_LIGHTEN;
        case BlendOp::ColorDodge:
          return CAIRO_OPERATOR_COLOR_DODGE;
        case BlendOp::ColorBurn:
          return CAIRO_OPERATOR_COLOR_BURN;
        case BlendOp::HardLight:
          return CAIRO_OPERATOR_HARD_LIGHT;
        case BlendOp::SoftLight:
          return CAIRO_OPERATOR_SOFT_LIGHT;
        case BlendOp::Difference:
          return CAIRO_OPERATOR_DIFFERENCE;
        case BlendOp::Exclusion:
          return CAIRO_OPERATOR_EXCLUSION;
        case BlendOp::Hue:
          return CAIRO_OPERATOR_HSL_HUE;
        case BlendOp::Saturation:
          return CAIRO_OPERATOR_HSL_SATURATION;
        case BlendOp::Color:
          return CAIRO_OPERATOR_HSL_COLOR;
        case BlendOp::Luminosity:
          return CAIRO_OPERATOR_HSL_LUMINOSITY;
      }

      assert(false);
      return CAIRO_OPERATOR_SOURCE;
    }

  }

  Canvas::Canvas(Surface& surface)
  : m_context(cairo_create(surface.m_surface))
  {
    assert(m_context);
  }

  Canvas::~Canvas() {
    if (m_context != nullptr) {
      cairo_destroy(m_context);
    }
  }

  Canvas::Canvas(Canvas&& other) noexcept
  : m_context(std::exchange(other.m_context, nullptr))
  {

  }

  Canvas& Canvas::operator=(Canvas&& other) noexcept {
    std::swap(m_context, other.m_context);
    return *this;
  }

  void Canvas::save() {
    cairo_save(m_context);
  }

  void Canvas::restore() {
    cairo_restore(m_context);
  }

  void Canvas::translate(gf::Vector2d t) {
    cairo_translate(m_context, t.x, t.y);
  }

  void Canvas::scale(gf::Vector2d s) {
    cairo_scale(m_context, s.x, s.y);
  }

  void Canvas::rotate(double angle) {
    cairo_rotate(m_context, angle);
  }

  void Canvas::clipRect(gf::RectD rect, ClipOp op) {
    cairo_rectangle(m_context, rect.left, rect.top, rect.width, rect.height);
    cairo_set_fill_rule(m_context, translateClipOp(op));
    cairo_clip(m_context);
  }

  void Canvas::clipCircle(gf::CircD circ, ClipOp op) {
    cairo_arc(m_context, circ.center.x, circ.center.y, circ.radius, 0.0, 2.0 * gf::Pi);
    cairo_set_fill_rule(m_context, translateClipOp(op));
    cairo_clip(m_context);
  }

  void Canvas::clipPath(const Path& path, ClipOp op) {
    setPath(path);
    cairo_set_fill_rule(m_context, translateClipOp(op));
    cairo_clip(m_context);
  }

//   void Canvas::drawColor(Color4d color, BlendOp op) {
//
//   }

  void Canvas::drawPaint(const Paint& paint) {
    CanvasGuard guard(*this);
    setContextFromPaint(paint);
    cairo_paint(m_context);
  }

  void Canvas::drawLine(gf::Vector2d p0, gf::Vector2d p1, const Paint& paint) {
    Path path;
    path.moveTo(p0);
    path.lineTo(p1);
    drawPath(path, paint);
  }

  void Canvas::drawRect(gf::RectD rect, const Paint& paint) {
    CanvasGuard guard(*this);
    setContextFromPaint(paint);
    cairo_rectangle(m_context, rect.left, rect.top, rect.width, rect.height);
    applyStyle(paint);
  }

  void Canvas::drawCircle(gf::CircD circ, const Paint& paint) {
    drawArc(circ, 0, 2 * gf::Pi, paint);
  }

  void Canvas::drawArc(gf::CircD circ, double angle0, double angle1, const Paint& paint) {
    CanvasGuard guard(*this);
    setContextFromPaint(paint);
    cairo_arc(m_context, circ.center.x, circ.center.y, circ.radius, angle0, angle1);
    applyStyle(paint);
  }

//   void Canvas::drawRoundedRect(gf::RectD rect, gf::Vector2d radius, const Paint& paint) {
//     assert(false && "TODO");
//   }

  void Canvas::drawPath(const Path& path, const Paint& paint) {
    CanvasGuard guard(*this);
    setContextFromPaint(paint);
    setPath(path);
    applyStyle(paint);
  }


  void Canvas::setContextFromPaint(const Paint& paint) {
    auto color = paint.getColor();
    cairo_set_source_rgba(m_context, color.r, color.g, color.b, color.a);

    auto pattern = paint.getPattern();

    if (pattern != nullptr) {
      cairo_set_source(m_context, pattern->m_pattern);
    }

    if (paint.getStyle() == Style::Stroke || paint.getStyle() == Style::StrokeAndFill) {
      cairo_set_line_width(m_context, paint.getStrokeWidth());
      cairo_set_miter_limit(m_context, paint.getStrokeMiter());
      cairo_set_line_cap(m_context, translateCap(paint.getStrokeCap()));
      cairo_set_line_join(m_context, translateJoin(paint.getStrokeJoin()));
    }

    cairo_set_antialias(m_context, translateAntiAlias(paint.getAntiAlias()));
    cairo_set_operator(m_context, translateBlendOp(paint.getBlendOp()));
  }

  void Canvas::setPath(const Path& path) {
    auto it = path.m_points.begin();

    for (auto command : path.m_commands) {
      switch (command) {
        case Path::Element::None:
          assert(false);
          break;

        case Path::Element::MoveTo: {
          auto p = *it++;
          cairo_move_to(m_context, p.x, p.y);
          break;
        }

        case Path::Element::LineTo: {
          auto p = *it++;
          cairo_line_to(m_context, p.x, p.y);
          break;
        }

        case Path::Element::CubicTo: {
          auto p1 = *it++;
          auto p2 = *it++;
          auto p3 = *it++;
          cairo_curve_to(m_context, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
          break;
        }

        case Path::Element::Close:
          cairo_close_path(m_context);
          break;
      }
    }
  }


  void Canvas::applyStyle(const Paint& paint) {
    switch (paint.getStyle()) {
      case Style::Stroke:
        cairo_stroke(m_context);
        break;
      case Style::Fill:
        cairo_fill(m_context);
        break;
      case Style::StrokeAndFill:
        cairo_fill_preserve(m_context);
        cairo_stroke(m_context);
        break;
    }
  }

}
