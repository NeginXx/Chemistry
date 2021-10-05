#pragma once
#include "main.h"
#include "render.h"

class CoordinateSystem {
 public:
 	CoordinateSystem(Point2D<size_t> left_corner,
                   Point2D<size_t> right_corner,
                   Point2D<float> min_coord,
                   Point2D<float> max_coord,
                   Color axes_color = {});
  Point2D<size_t> ConvertCoordinate(Point2D<float> a);
  size_t ConvertXLength(float len);
  size_t ConvertYLength(float len);
 	void DrawFunction(float (*func)(float), Render& render, Color color = {});
  void Draw(Render& render, Color background_color);

 private:
  Point2D<size_t> left_corner_;
  Point2D<size_t> right_corner_;
  Point2D<float> min_coord_;
  Point2D<float> max_coord_;
  Point2D<size_t> axes_center_;
  Point2D<size_t> point_1_1_;
  Color axes_color_;
};