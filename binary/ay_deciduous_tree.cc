#include <cstdlib>
#include <iostream>

#include <gf/Random.h>

#include <animactory/Canvas.h>
#include <animactory/Surface.h>

namespace {

  constexpr int Size = 256;

  constexpr double RadiusMin = 0.7 * Size / 2;
  constexpr double RadiusMax = 1.0 * Size / 2;

  constexpr double BallLengthMin = 0.2 * Size / 2;
  constexpr double BallLengthMax = 0.5 * Size / 2;

  constexpr double StrokeWidth = Size / 40.0;

  constexpr int FaceCount = 7;
  constexpr int BallCount = 9;

  constexpr double Transparency = 0.7;

  constexpr gf::Color4d TreeGreen{ 0.0, 0.75, 0.0, Transparency };

  ay::Path createSimpleFoliagePath(gf::Random& random) {
    double radiusMinMax = (2 * RadiusMin + RadiusMax) / 3;
    double radiusMaxMin = (RadiusMin + 2 * RadiusMax) / 3;

    std::vector<gf::Vector2d> points;

    for (int i = 0; i < FaceCount; ++i) {
      double radius = random.computeUniformFloat(RadiusMin, radiusMinMax);
      points.push_back(radius * gf::unit(2.0 * gf::Pi * i / FaceCount));
    }

    ay::Path path;
    path.moveTo(points[0]);

    for (std::size_t i = 1; i < points.size(); ++i) {
      double radius = random.computeUniformFloat(radiusMaxMin, RadiusMax);
      auto p1 = radius * gf::unit(2.0 * gf::Pi * (i - 0.7) / FaceCount);
      auto p2 = radius * gf::unit(2.0 * gf::Pi * (i - 0.3) / FaceCount);
      path.cubicTo(p1, p2, points[i]);
    }

    {
      double radius = random.computeUniformFloat(radiusMaxMin, RadiusMax);
      auto p = radius * gf::unit(2.0 * gf::Pi * -0.5 / FaceCount);
      path.cubicTo(p, p, points[0]);
    }

    path.close();

    return path;
  }

  ay::Pattern createShadowPattern(gf::Vector2d center, double radius) {
    ay::Pattern pattern = ay::Pattern::makeRadial(gf::CircD(center, 0.0), gf::CircD(center, radius));
    pattern.addColorStop(0.0, { 0.0, 0.5, 0.0, 0.8 });
    pattern.addColorStop(1.0, { 0.0, 0.5, 0.0, 0.2 });
    return pattern;
  }

  void simpleFoliage(ay::Canvas& canvas, gf::Random& random) {
    ay::CanvasGuard guard(canvas);

    auto path = createSimpleFoliagePath(random);

    ay::Paint paint;

    paint.setStyle(ay::Style::Fill);
    paint.setColor(TreeGreen);
    canvas.drawPath(path, paint);

    auto pattern = createShadowPattern({ 0.0, 0.0 }, RadiusMax);
    paint.setPattern(&pattern);
    canvas.clipPath(path);
    canvas.drawPaint(paint);

    paint.setStyle(ay::Style::Stroke);
    paint.setPattern(nullptr);
    paint.setColor(gf::ColorD::darker(TreeGreen));
    paint.setStrokeWidth(StrokeWidth);
    paint.setStrokeJoin(ay::Join::Miter);
    canvas.drawPath(path, paint);
  }

  struct Ball {
    double length;
    double radius;
    double angle;
  };

  void ballFoliage(ay::Canvas& canvas, gf::Random& random) {
    ay::CanvasGuard guard(canvas);

    std::vector<Ball> balls;

    for (int i = 0; i < BallCount; ++i) {
      double length = random.computeUniformFloat(BallLengthMin, BallLengthMax);
      double angle = i * 2.0 * gf::Pi / BallCount + random.computeUniformFloat(0.0, 2.0 * gf::Pi / BallCount);

      double localRadiusMax = std::min(RadiusMax - length - 2 * StrokeWidth, 1.5 * length);
      double radius = random.computeUniformFloat(localRadiusMax * 0.8, localRadiusMax);
      balls.push_back({ length, radius, angle });
    }

    std::sort(balls.begin(), balls.end(), [](const Ball& lhs, const Ball& rhs) {
      return lhs.length > rhs.length;
    });

    ay::Paint paint;
    paint.setStrokeCap(ay::Cap::Round);

    for (auto ball : balls) {
      ay::CanvasGuard loopGuard(canvas);

      auto center = ball.length * gf::unit(ball.angle);
      gf::CircD circle(center, ball.radius);

      paint.setStyle(ay::Style::Fill);
      paint.setColor(TreeGreen * gf::ColorD::Opaque(0.5));
      canvas.drawCircle(circle, paint);

      auto pattern = createShadowPattern(center, ball.radius);
      paint.setPattern(&pattern);
      canvas.clipCircle(circle);
      canvas.drawPaint(paint);

      paint.setPattern(nullptr);
      paint.setStyle(ay::Style::Stroke);
      paint.setColor(gf::ColorD::darker(TreeGreen));
      paint.setStrokeWidth(StrokeWidth);
      canvas.drawArc(circle, ball.angle - gf::Pi2, ball.angle + gf::Pi2, paint);

    }

  }


}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ay_tree <filename>\n";
    return EXIT_FAILURE;
  }

  gf::Random random;

  ay::Surface surface({ Size, Size });
  ay::Canvas canvas(surface);
  canvas.translate({ Size / 2.0, Size / 2.0 });

  simpleFoliage(canvas, random);
  ballFoliage(canvas, random);


  surface.exportToPng(argv[1]);
  return EXIT_SUCCESS;
}
