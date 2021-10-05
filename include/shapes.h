#pragma once
#include "main.h"
#include "render.h"
#include "CoordinateSystem.h"

class PixelRectangle {
 public:
  PixelRectangle(Point2D<size_t> left_corner,
                 Point2D<size_t> right_corner,
                 Color color = {});

  void Draw(Render& render);

 private:
  Point2D<size_t> left_corner_;
  Point2D<size_t> right_corner_;
  Color color_;
};

class PixelVector {
 public:
  PixelVector(Point2D<size_t> origin,
              Point2D<size_t> end,
              Color color = {});
  void Normalize(float length);
  void Rotate(float angle);
  void Draw(Render& render);
  Point2D<size_t>& GetOrigin();
  Point2D<size_t>& GetEnd();

 private:
  Point2D<size_t> origin_;
  Point2D<size_t> end_;
  Color color_;
};

class Shape {
 public:
  virtual void Draw(CoordinateSystem& coord_sys, Render& render) = 0;
  Color GetColor();

 protected:
	Color color_;
};

class Line : public virtual Shape {
 public:
 	Line(Point2D<float> a, Point2D<float> b,
 		   Color color = {}, bool is_thick = false);
  Point2D<float> GetA();
  Point2D<float> GetB();
 	void Draw(CoordinateSystem& coord_sys, Render& render) override;

 protected:
 	Point2D<float> a_;
 	Point2D<float> b_;
  bool is_thick_;
};

class Rectangle : public virtual Shape {
 public:
 	Rectangle(Point2D<float> left_corner,
            Point2D<float> right_corner,
            Color color = {});
 	void Draw(CoordinateSystem& coord_sys, Render& render) override;
  Point2D<float> GetCenter();
  float GetWidth();
  float GetHeight();

 protected:
  Point2D<float> left_corner_;
	Point2D<float> right_corner_;
};

class Vector : public virtual Shape {
 public:
  Vector(Point2D<float> origin,
         Point2D<float> coordinate,
         Color color = {});
  void Rotate(float angle);
  void Draw(CoordinateSystem& coord_sys, Render& render) override;

 private:
  Point2D<float> origin_;
  Point2D<float> coordinate_;
};

class Circle : public virtual Shape {
 public:
  Circle(Point2D<float> center,
         float radius,
         Color color = {});
  void Draw(CoordinateSystem& coord_sys, Render& render) override;
  Point2D<float> GetCenter();
  float GetRadius();

 protected:
  Point2D<float> center_;
  float radius_;
};

enum ShapeType {
  kCircle,
  kRectangle,
  kWall
};

class PhysicalShape : public virtual Shape {
 public:
  PhysicalShape(ShapeType type, PhysicalShape* collision_partner);
  virtual void Move(float dt) = 0;
  const ShapeType type;
  PhysicalShape* collision_partner;
};

class PhysicalRectangle : public PhysicalShape, public Rectangle {
 public:
  PhysicalRectangle(Point2D<float> left_corner,
                    Point2D<float> right_corner,
                    Vec2D<float> velocity,
                    float mass,
                    Color color = {});
  PhysicalRectangle(Point2D<float> center,
                    float width,
                    float height,
                    Vec2D<float> velocity,
                    float mass,
                    Color color = {});
  void Move(float dt) override;
  Vec2D<float> GetVelocity();
  float GetMass();
  void SetVelocity(Vec2D<float> vec);

 private:
  Vec2D<float> velocity_;
  float mass_;
};

class PhysicalCircle : public PhysicalShape, public Circle {
 public:
  PhysicalCircle(Point2D<float> center,
                 Vec2D<float> velocity,
                 float mass,
                 float radius,
                 Color color = {});
  void Move(float dt) override;
  Vec2D<float> GetVelocity();
  float GetMass();
  void SetVelocity(Vec2D<float> vec);

 private:
  Vec2D<float> velocity_;
  float mass_;
};

class PhysicalWall : public PhysicalShape, public Line {
 public:
  PhysicalWall(Point2D<float> a,
               Point2D<float> b,
               Color color = {});
  void Move(float dt) override;
};

// struct ShapePointer {
//   ShapePointer(Shape* ptr_) {
//     ptr = ptr_;
//   }
//   ~ShapePointer() {

//   }
//   Shape* ptr = nullptr;
// }