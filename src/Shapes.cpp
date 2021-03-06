#include <math.h>
#include <stdio.h>
#include "../include/Shapes.h"

const float kAbsoluteZeroTemperature = 0.1;
const unsigned char kColorDiff = 20;

PixelRectangle::PixelRectangle(const Point2D<size_t>& left_corner,
                               const Point2D<size_t>& right_corner,
                               const Color& color) {
	left_corner_ = left_corner;
	right_corner_ = right_corner;
	color_ = color;
}

void PixelRectangle::Draw(Render& render) const {
	render.DrawRectangle(left_corner_, right_corner_, color_);
}




PixelVector::PixelVector(const Point2D<size_t>& origin,
	                       const Point2D<size_t>& end,
	                       const Color& color) {
	origin_ = origin;
	end_ = end;
	color_ = color;
}

Point2D<size_t> PixelVector::GetOrigin() const {
	return origin_;
}

Point2D<size_t> PixelVector::GetEnd() const {
	return end_;
}

void PixelVector::Rotate(float angle) {
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

  end_ = Point2D<size_t>{vec_coord.x + origin_.x, vec_coord.y + origin_.y};
}

void PixelVector::Normalize(float length) {
	float vec_length = sqrt(pow((float)end_.x - (float)origin_.x, 2) +
		                      pow((float)end_.y - (float)origin_.y, 2));
	float factor = length / vec_length;
	end_.x = origin_.x + ((int)end_.x - (int)origin_.x) * factor;
	end_.y = origin_.y + ((int)end_.y - (int)origin_.y) * factor;
}

void PixelVector::Draw(Render& render) const {
	// vec_end - end of auxiliary vector
	Point2D<size_t> vec_end = { end_.x - ((int)end_.x - (int)origin_.x) / 10,
	                            end_.y + ((int)origin_.y - (int)end_.y) / 10 };
	PixelVector vec(end_, vec_end, color_);
	vec.Normalize(30);
	float angle = 20.0 / 180.0 * M_PI;
	vec.Rotate(angle);
	render.DrawLine(end_, vec.GetEnd(), color_);
	vec.Rotate(-2 * angle);
	render.DrawLine(end_, vec.GetEnd(), color_);
	render.DrawLine(origin_, end_, color_);
}



Color Shape::GetColor() const {
	return color_;
}



Line::Line(const Point2D<float>& a, const Point2D<float>& b,
	         const Color& color, bool is_thick) {
	a_ = a;
	b_ = b;
	color_ = color;
	is_thick_ = is_thick;
}

Point2D<float> Line::GetA() const {
	return a_;
}

Point2D<float> Line::GetB() const {
	return b_;
}

void Line::SetA(const Point2D<float>& a) {
	a_ = a;
}

void Line::SetB(const Point2D<float>& b) {
	b_ = b;
}

void Line::Draw(const CoordinateSystem& coord_sys, Render& render) const {
	Point2D<size_t> a = coord_sys.ConvertCoordinate(a_);
	Point2D<size_t> b = coord_sys.ConvertCoordinate(b_);
	render.DrawLine(a, b, color_);
	if (is_thick_) {
		Point2D<size_t> ofs = {0, 1};
		if (a.x != b.x) {
			render.DrawLine(a + ofs, b + ofs, color_);
			render.DrawLine(a - ofs, b - ofs, color_);
		} else {
			ofs = Point2D<size_t>{1, 0};
			render.DrawLine(a + ofs, b + ofs, color_);
			render.DrawLine(a - ofs, b - ofs, color_);
		}
	}
}




Rectangle::Rectangle(const Point2D<float>& left_corner,
                     const Point2D<float>& right_corner,
                     const Color& color) {
	left_corner_ = left_corner;
	right_corner_ = right_corner;
	color_ = color;
}

void Rectangle::Draw(const CoordinateSystem& coord_sys, Render& render) const {
	render.DrawRectangle(coord_sys.ConvertCoordinate(left_corner_),
		                   coord_sys.ConvertCoordinate(right_corner_),
		                   color_);
}

Point2D<float> Rectangle::GetLeftCorner() const {
	return left_corner_;
}

Point2D<float> Rectangle::GetRightCorner() const {
	return right_corner_;
}

Point2D<float> Rectangle::GetCenter() const {
	return left_corner_ + (right_corner_ - left_corner_) / (float)2.0;
}

float Rectangle::GetWidth() const {
	return right_corner_.x - left_corner_.x;
}

float Rectangle::GetHeight() const {
	return left_corner_.y - right_corner_.y;
}




Vector::Vector(const Point2D<float>& origin,
               const Point2D<float>& coordinate,
               const Color& color) {
  origin_ = origin;
  coordinate_ = coordinate;
  color_ = color;
}

void Vector::Rotate(float angle) {
  float& x = coordinate_.x;
  float& y = coordinate_.y;
  float temp = x;
  x = x * cosf(angle) - y * sinf(angle);
  y = temp * sinf(angle) + y * cosf(angle);
}

void Vector::Draw(const CoordinateSystem& coord_sys,
                  Render& render) const {
  PixelVector vec(coord_sys.ConvertCoordinate(origin_),
                  coord_sys.ConvertCoordinate(origin_ + coordinate_),
                  color_);
  vec.Draw(render);
}




Circle::Circle(const Point2D<float>& center,
               float radius,
               const Color& color) {
  center_ = center;
  radius_ = radius;
  color_ = color;
}

Point2D<float> Circle::GetCenter() const {
	return center_;
}

float Circle::GetRadius() const {
	return radius_;
}

inline bool PointIsInCircle(size_t x, size_t y,
	                          const Point2D<size_t>& center,
	                          size_t radius) {
	return pow((double)x - (double)center.x, 2) + pow((double)y - (double)center.y, 2) - pow((double)radius, 2) <= 0;
}

void Circle::Draw(const CoordinateSystem& coord_sys,
                  Render& render) const {
	// l_c - left_corner, r_c - right_corner
	size_t width = coord_sys.ConvertXLength(radius_);
	size_t height = coord_sys.ConvertYLength(radius_);
	size_t radius = width > height ? width : height;
  Point2D<size_t> center = coord_sys.ConvertCoordinate(center_);
  Point2D<size_t> l_c = center - Point2D<size_t>{radius, radius};
  Point2D<size_t> r_c = center + Point2D<size_t>{radius, radius};
	for (size_t y = l_c.y; y <= r_c.y; ++y) {
		for (size_t x = l_c.x; x <= r_c.x; ++x) {
		  if (PointIsInCircle(x, y, center, radius)) {
		    render.DrawPoint({x, y}, color_);
	    }
		}
	}
}




PhysicalShape::PhysicalShape(ShapeType type_, PhysicalShape* collision_partner_)
	: type(type_), collision_partner(collision_partner_) {}

PhysicalRectangle::PhysicalRectangle(const Point2D<float>& left_corner,
                                     const Point2D<float>& right_corner,
                                     const Vec2D<float>& velocity,
                                     float mass,
                                     const Color& color)
	: PhysicalShape(kRectangle, nullptr),
	  Rectangle(left_corner, right_corner, color),
    velocity_(velocity), mass_(mass) {}

PhysicalRectangle::PhysicalRectangle(const Point2D<float>& center,
                                     float width,
                                     float height,
                                     const Vec2D<float>& velocity,
                                     float mass,
                                     const Color& color)
	: PhysicalShape(kRectangle, nullptr),
		Rectangle(center + Point2D<float>(-width / 2, height / 2),
	            center + Point2D<float>(width / 2, -height / 2), color),
		velocity_(velocity),
		mass_(mass) {}

void PhysicalRectangle::Move(float dt) {
	left_corner_.x += velocity_.x * dt;
	right_corner_.x += velocity_.x * dt;
	left_corner_.y += velocity_.y * dt;
	right_corner_.y += velocity_.y * dt;
}

Vec2D<float> PhysicalRectangle::GetVelocity() const {
	return velocity_;
}

void PhysicalRectangle::SetVelocity(const Vec2D<float>& vec) {
	velocity_ = vec;
}

float PhysicalRectangle::GetMass() const {
	return mass_;
}




PhysicalCircle::PhysicalCircle(const Point2D<float>& center,
	                             const Vec2D<float>& velocity,
                               float mass,
                               float radius,
                               const Color& color)
	: PhysicalShape(kCircle, nullptr),
	  Circle(center, radius, color),
    velocity_(velocity), mass_(mass) {}

void PhysicalCircle::Move(float dt) {
	center_.x += velocity_.x * dt;
	center_.y += velocity_.y * dt;
}

Vec2D<float> PhysicalCircle::GetVelocity() const {
	return velocity_;
}

void PhysicalCircle::SetVelocity(const Vec2D<float>& vec) {
	velocity_ = vec;
}

float PhysicalCircle::GetMass() const {
	return mass_;
}




PhysicalWall::PhysicalWall(const Point2D<float>& a,
	                         const Point2D<float>& b,
                           const Color& color,
                           float temperature)
	: PhysicalShape(kWall, nullptr),
	  Line(a, b, color, true), temperature_(temperature) {}

void PhysicalWall::Move(float dt) {}

void AddToColorWithoutOverflow(unsigned char& c) {
	c = (c + kColorDiff >= 255 ? 255 : c + kColorDiff);
}

void SubFromColorWithoutOverflow(unsigned char& c) {
	c = (c - kColorDiff < 0 ? 0 : c - kColorDiff);
}

void PhysicalWall::IncreaseTemperature(float diff_temp) {
	if (temperature_ + diff_temp >= kAbsoluteZeroTemperature) {
		temperature_ += diff_temp;
	} else {
		temperature_ = kAbsoluteZeroTemperature;
	}
	// printf("temp = %f\n", temperature_);
	if (diff_temp > 0) {
		if (color_.blue == 0) {
			AddToColorWithoutOverflow(color_.red);
		} else {
			SubFromColorWithoutOverflow(color_.blue);
		}
	}

	else {
		if (color_.red == 0) {
			AddToColorWithoutOverflow(color_.blue);
		} else {
			SubFromColorWithoutOverflow(color_.red);
		}
	}
}

float PhysicalWall::GetTemperature() const {
	return temperature_;
}