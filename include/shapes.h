#pragma once

class Render;

class Shape {
 public:
 	virtual void Draw(Render& render) = 0;

 protected:
	Color color_;
};

class Line : public Shape {
 public:
 	Line(Point2D<size_t> a, Point2D<size_t> b,
 		   Color color);

 	void Draw(Render& render) override;

 private:
 	Point2D<size_t> a_;
 	Point2D<size_t> b_;
};

class Rectangle : public Shape {
 public:
 	Rectangle(size_t x, size_t y, size_t width,
 		        size_t height, Color color);

 	void Draw(Render& render) override;

 private:
	Point2D<size_t> left_corner_;
	size_t width_ = 0;
	size_t height_ = 0;
};

class Vector : public Shape {
 public:
 	Vector(Point2D<size_t> origin,
 		     Point2D<size_t> end,
 		     Color color);
  void Normalize(float length);
 	void Rotate(float angle);
 	void Draw(Render& render) override;
 	Point2D<size_t>& GetOrigin();
 	Point2D<size_t>& GetEnd();

 private:
 	Point2D<size_t> origin_;
  Point2D<size_t> end_;
};

class Axes : public Shape {
 public:
 	Axes(Point2D<size_t> x1,
 	     Point2D<size_t> y1,
 	     Point2D<size_t> x2,
 	     Point2D<size_t> y2,
 	     Point2D<float> max_coord,
 	     Color color);
 	void Draw(Render& render) override;
  friend class CoordinateSystem;

 private:
  Point2D<size_t> x1_;
 	Point2D<size_t> y1_;
 	Point2D<size_t> x2_;
 	Point2D<size_t> y2_;
 	Point2D<float> max_coord_;
	Point2D<size_t> point_1_1_;
};