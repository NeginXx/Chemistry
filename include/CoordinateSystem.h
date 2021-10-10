#pragma once
#include "main.h"
#include "Render.h"

class CoordinateSystem {
 public:
 	CoordinateSystem(const Point2D<size_t>& left_corner,
                   const Point2D<size_t>& right_corner,
                   const Point2D<float>& min_coord,
                   const Point2D<float>& max_coord,
                   const Color& axes_color = {});
  Point2D<size_t> ConvertCoordinate(const Point2D<float>& a) const;
  Point2D<float> ConvertCoordinate(const Point2D<size_t>& a) const;
  size_t ConvertXLength(float len) const;
  size_t ConvertYLength(float len) const;
 	void DrawFunction(float (*func)(float), Render& render,
                    const Color& color = {}) const;
  void Draw(Render& render, const char* ox_str = "x",
            const char* oy_str = "y",
            const Color& background_color = {}) const;

 private:
  Point2D<size_t> left_corner_;
  Point2D<size_t> right_corner_;
  Point2D<float> min_coord_;
  Point2D<float> max_coord_;
  Point2D<size_t> axes_center_;
  Point2D<size_t> point_1_1_;
  Color axes_color_;
};