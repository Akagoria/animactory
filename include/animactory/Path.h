#ifndef AY_PATH_H
#define AY_PATH_H

#include <vector>

#include <gf/Vector.h>

namespace ay {

  class Path {
  public:
    enum class Element {
      None,
      MoveTo,
      LineTo,
//       QuadTo,
      CubicTo,
      Close,
    };

    void moveTo(gf::Vector2d point);
    void lineTo(gf::Vector2d point);
//     void quadTo(gf::Vector2d c, gf::Vector2d endPoint);
    void cubicTo(gf::Vector2d c1, gf::Vector2d c2, gf::Vector2d endPoint);
    void close();

    gf::Vector2d lastPoint() const;

  private:
    friend class Canvas;
    std::vector<Element> m_commands;
    std::vector<gf::Vector2d> m_points;
  };


}

#endif // AY_PATH_H

