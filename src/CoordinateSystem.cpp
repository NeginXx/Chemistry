#include <math.h>
#include "../include/main.h"
#include "../include/shapes.h"
#include "../include/render.h"
#include "../include/CoordinateSystem.h"

VectorInCoordinateSystem::
VectorInCoordinateSystem(Point2D<float> origin,
	                       Point2D<float> coordinate,
 		                     Color color) {
	origin_ = origin;
	coordinate_ = coordinate;
	color_ = color;
}

void VectorInCoordinateSystem::Rotate(float angle) {
	float& x = coordinate_.x;
	float& y = coordinate_.y;
	float temp = x;
  x = x * cosf(angle) - y * sinf(angle);
  y = temp * sinf(angle) + y * cosf(angle);
}

CoordinateSystem::CoordinateSystem(Axes* axes) {
	axes_ = axes;
}

void CoordinateSystem::DrawVector(
	   VectorInCoordinateSystem& vector,
     Render& render) {
	Vector vector_shape(ConvertCoordinate(vector.origin_),
		                  ConvertCoordinate(vector.origin_ + vector.coordinate_),
	                    vector.color_);
  vector_shape.Draw(render);
}

Point2D<size_t> CoordinateSystem::ConvertCoordinate(Point2D<float> a) {
	Point2D<size_t> center = {axes_->x2_.x, axes_->x1_.y};
	Point2D<size_t> p11 = axes_->point_1_1_;
	return {center.x + (int)(a.x * (float)(p11.x - center.x)),
	        center.y - (int)(a.y * (float)(center.y - p11.y))};
}

void CoordinateSystem::DrawFunction(float (*func)(float),
	                                  Render& render, Color color) {
	float temp = -(float)(axes_->x2_.x - axes_->x1_.x) / (float)(axes_->point_1_1_.x - axes_->x2_.x);
	Point2D<float> prev = {temp, func(temp)}; // previous point
	render.DrawPoint(ConvertCoordinate(prev), color);
	float max_x = axes_->max_coord_.x;
	float d_x = max_x / 50;
	Point2D<float> cur = {}; // current point
	for (cur.x = prev.x + d_x; cur.x < max_x; cur.x += d_x) {
		cur.y = func(cur.x);
		if (abs(cur.y) <= axes_->max_coord_.y) {
		  render.DrawPoint(ConvertCoordinate(cur), color);
		  render.DrawLine(ConvertCoordinate(cur), ConvertCoordinate(prev), color);
		}
		prev = cur;
	}
}