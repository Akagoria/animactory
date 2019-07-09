#include <animactory/Path.h>

namespace ay {

  void Path::moveTo(gf::Vector2d point) {
    m_commands.push_back(Element::MoveTo);
    m_points.push_back(point);
  }

  void Path::lineTo(gf::Vector2d point) {
    m_commands.push_back(Element::LineTo);
    m_points.push_back(point);
  }

//   void Path::quadTo(gf::Vector2d c, gf::Vector2d endPoint) {
//     m_commands.push_back(Element::QuadTo);
//     m_points.push_back(c);
//     m_points.push_back(endPoint);
//   }

  void Path::cubicTo(gf::Vector2d c1, gf::Vector2d c2, gf::Vector2d endPoint) {
    m_commands.push_back(Element::CubicTo);
    m_points.push_back(c1);
    m_points.push_back(c2);
    m_points.push_back(endPoint);
  }

  void Path::close() {
    m_commands.push_back(Element::Close);
  }

  gf::Vector2d Path::lastPoint() const {
    if (!m_points.empty()) {
      return m_points.back();
    }

    return { 0.0, 0.0 };
  }

}
