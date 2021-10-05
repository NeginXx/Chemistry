#include <math.h>
#include <stdio.h>
#include "../include/shapes.h"
#include "../include/CoordinateSystem.h"

static Point2D<size_t> frame_ofs = {10, 10};

CoordinateSystem::
CoordinateSystem(Point2D<size_t> left_corner,
                 Point2D<size_t> right_corner,
                 Point2D<float> min_coord,
                 Point2D<float> max_coord,
                 Color axes_color) {
	assert(left_corner.x < right_corner.x);
	assert(left_corner.y < right_corner.y);
	assert(min_coord.x < 0 && min_coord.y < 0);
	assert(max_coord.x > 0 && max_coord.y > 0);

  left_corner_ = left_corner + frame_ofs;
  right_corner_ = right_corner - frame_ofs;
  min_coord_ = min_coord;
  max_coord_ = max_coord;
  axes_color_ = axes_color;

  Point2D<size_t> l_c = left_corner_;
  Point2D<size_t> r_c = right_corner_;
  Point2D<float> min_c = min_coord_;
  Point2D<float> max_c = max_coord_;
  size_t len_pixels = r_c.x - l_c.x;
  float len_float = max_c.x - min_c.x;
  axes_center_.x = l_c.x + (int)((float)len_pixels * (-min_c.x / len_float));
  len_pixels = r_c.y - l_c.y;
  len_float = max_c.y - min_c.y;
  axes_center_.y = l_c.y + (int)((float)len_pixels * (max_c.y / len_float));
  Point2D<size_t> center = axes_center_;
  point_1_1_ = {center.x + (int)((float)(r_c.x - center.x) * (1 / max_c.x)),
  							center.y - (int)((float)(center.y - l_c.y) * (1 / max_c.y))};
}

size_t CoordinateSystem::ConvertXLength(float len) {
	size_t lenn = (size_t)((float)(point_1_1_.x - axes_center_.x) * len);
	return lenn > 1 ? lenn : 1;
}

size_t CoordinateSystem::ConvertYLength(float len) {
	size_t lenn = (size_t)((float)(axes_center_.y - point_1_1_.y) * len);
	return lenn > 1 ? lenn : 1;
}

size_t Min(size_t a, size_t b) {
	return a < b ? a : b;
}

Point2D<size_t> CoordinateSystem::ConvertCoordinate(Point2D<float> a) {
	Point2D<size_t> p11 = point_1_1_;
	Point2D<size_t> center = axes_center_;
	return {Min(center.x + (int)(a.x * (float)(p11.x - center.x)), 3000),
	        Min(center.y - (int)(a.y * (float)(center.y - p11.y)), 3000)};
}

void CoordinateSystem::DrawFunction(float (*func)(float),
	                                  Render& render, Color color) {
	Point2D<float> prev = {min_coord_.x, func(min_coord_.x)}; // previous point
	render.DrawPoint(ConvertCoordinate(prev), color);
	float max_x = max_coord_.x;
	float d_x = max_x / 50;
	Point2D<float> cur = {}; // current point
	for (cur.x = prev.x + d_x; cur.x < max_x; cur.x += d_x) {
		cur.y = func(cur.x);
		if (abs(cur.y) <= max_coord_.y) {
		  render.DrawPoint(ConvertCoordinate(cur), color);
		  render.DrawLine(ConvertCoordinate(cur), ConvertCoordinate(prev), color);
		}
		prev = cur;
	}
}

void CoordinateSystem::Draw(Render& render, Color background_color = {}) {
  Point2D<size_t> l_c = left_corner_;
  Point2D<size_t> r_c = right_corner_;
  Point2D<size_t> center = axes_center_;

	PixelRectangle rect(l_c - frame_ofs, r_c + frame_ofs, background_color);
	rect.Draw(render);

	PixelVector x_axis({l_c.x - frame_ofs.x, center.y}, {r_c.x + frame_ofs.x, center.y}, axes_color_);
	x_axis.Draw(render);
	PixelVector y_axis({center.x, r_c.y + frame_ofs.y}, {center.x, l_c.y - frame_ofs.y}, axes_color_);
	y_axis.Draw(render);

  // ------------------------------------------------------------------------------------
  // Drawing Ox labels

	render.DrawText("x", {r_c.x - 5, center.y - 35});
	render.DrawLine({r_c.x, center.y - 5},
		              {r_c.x, center.y + 5}, axes_color_);

  char num_str[10] = {};
  sprintf(num_str, "%lu\n", (size_t)round(max_coord_.x));
  render.DrawText(num_str, {r_c.x - 5, center.y + 10});

  render.DrawLine({point_1_1_.x - 5, center.y - 5},
  	              {point_1_1_.x - 5, center.y + 5}, axes_color_);
  render.DrawText("1", {point_1_1_.x - 8, center.y + 10});

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  // ------------------------------------------------------------------------------------
  // Drawing Oy labels

	render.DrawText("y", {center.x - 23, l_c.y - frame_ofs.y});
	render.DrawLine({center.x + 5, l_c.y},
		              {center.x - 5, l_c.y}, axes_color_);

  sprintf(num_str, "%lu\n", (size_t)round(max_coord_.y));
  render.DrawText(num_str, {center.x + 11, l_c.y - frame_ofs.y + 2});

  render.DrawLine({center.x + 5, point_1_1_.y},
  	              {center.x - 5, point_1_1_.y}, axes_color_);
  render.DrawText("1", {center.x - 18, point_1_1_.y - 12});

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  render.DrawText("0", {center.x - 20, center.y + 3});
}