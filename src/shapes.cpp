#include <math.h>
#include <stdio.h>
#include "../include/main.h"
#include "../include/render.h"
#include "../include/shapes.h"

Line::Line(Point2D<size_t> a, Point2D<size_t> b,
	         Color color = {}) {
	a_ = a;
	b_ = b;
	color_ = color;
}

void Line::Draw(Render& render) {
	render.DrawLine(a_, b_, color_);
}

Rectangle::Rectangle(size_t x, size_t y, size_t width,
	                   size_t height, Color color = {}) {
	left_corner_ = {x, y};
	width_ = width;
	height_ = height;
	color_ = color;
}

void Rectangle::Draw(Render& render) {
	render.DrawRectangle(left_corner_, width_,
		                   height_, color_);
}

Vector::Vector(Point2D<size_t> origin,
	             Point2D<size_t> end,
	             Color color = {}) {
	origin_ = origin;
	end_ = end;
	color_ = color;
}

Point2D<size_t>& Vector::GetOrigin() {
	return origin_;
}

Point2D<size_t>& Vector::GetEnd() {
	return end_;
}

void Vector::Rotate(float angle) {
	Point2D<int> vec_coord = {0, 0};

	if (end_.x > origin_.x) {
		vec_coord.x = end_.x - origin_.x;
	} else {
		vec_coord.x = -(int)(origin_.x - end_.x);
	}

	if (end_.y > origin_.y) {
		vec_coord.y = end_.y - origin_.y;
	} else {
		vec_coord.y = -(int)(origin_.y - end_.y);
	}

	int x = vec_coord.x;
  vec_coord.x = vec_coord.x * cosf(angle) - vec_coord.y * sinf(angle);
  vec_coord.y = x * sinf(angle) + vec_coord.y * cosf(angle);

  end_ = {vec_coord.x + origin_.x, vec_coord.y + origin_.y};
}

void Vector::Normalize(float length) {
	float vec_length = sqrt(pow((float)end_.x - (float)origin_.x, 2) +
		                      pow((float)end_.y - (float)origin_.y, 2));
	float factor = length / vec_length;
	end_.x = origin_.x + ((int)end_.x - (int)origin_.x) * factor;
	end_.y = origin_.y + ((int)end_.y - (int)origin_.y) * factor;
}

void Vector::Draw(Render& render) {
	// vec_end - end of auxiliary vector
	Point2D<size_t> vec_end = { end_.x - ((int)end_.x - (int)origin_.x) / 10,
	                            end_.y + ((int)origin_.y - (int)end_.y) / 10 };
	Vector vec(end_, vec_end, color_);
	vec.Normalize(30);
	float angle = 20.0 / 180.0 * M_PI;
	vec.Rotate(angle);

	Line arrow_line1(end_, vec.GetEnd(), color_);
	vec.Rotate(-2 * angle);
	Line arrow_line2(end_, vec.GetEnd(), color_);

	arrow_line1.Draw(render);
	arrow_line2.Draw(render);
	Line main_line(origin_, end_, color_);
	main_line.Draw(render);
}

Axes::Axes(Point2D<size_t> x1,
           Point2D<size_t> y1,
           Point2D<size_t> x2,
           Point2D<size_t> y2,
           Point2D<float> max_coord,
           Color color = {}) {
	x1_ = x1;
	y1_ = y1;
	x2_ = x2;
	y2_ = y2;
	max_coord_ = max_coord;
	color_ = color;
}

void Axes::Draw(Render& render) {
	Vector x_axis(x1_, y1_, color_);
	x_axis.Draw(render);
	Vector y_axis(x2_, y2_, color_);
	y_axis.Draw(render);

  // ------------------------------------------------------------------------------------
  // Drawing Ox labels

	render.DrawText("x", {y1_.x - 13, y1_.y - 35});
	Line line1({y1_.x - 10, y1_.y - 5},
		         {y1_.x - 10, y1_.y + 5}, color_);
	line1.Draw(render);

  char num_str[10] = {};
  sprintf(num_str, "%lu\n", (size_t)round(max_coord_.x));
  size_t max_x_coord = y1_.x - 10;
  render.DrawText(num_str, {y1_.x - 15, y1_.y + 10});

  point_1_1_.x = x2_.x + (size_t)((float)(max_x_coord - x2_.x) / max_coord_.x);
  Line line2({point_1_1_.x - 5, y1_.y - 5},
  	         {point_1_1_.x - 5, y1_.y + 5}, color_);
  line2.Draw(render);
  render.DrawText("1", {point_1_1_.x - 8, y1_.y + 10});

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  // ------------------------------------------------------------------------------------
  // Drawing Oy labels

	render.DrawText("y", {y2_.x - 20, y2_.y - 3});
	Line line3({y2_.x + 5, y2_.y + 10},
		         {y2_.x - 5, y2_.y + 10}, color_);
	line3.Draw(render);

  sprintf(num_str, "%lu\n", (size_t)round(max_coord_.y));
  size_t max_y_coord = y2_.y + 10;
  render.DrawText(num_str, {y2_.x + 10, y2_.y});

	point_1_1_.y = x1_.y - (size_t)((float)(x1_.y - max_y_coord) / max_coord_.y);
  Line line4({y2_.x + 5, point_1_1_.y},
  	         {y2_.x - 5, point_1_1_.y}, color_);
  line4.Draw(render);
  render.DrawText("1", {y2_.x - 18, point_1_1_.y - 12});

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  render.DrawText("0", {x2_.x - 20, x1_.y + 3});
}