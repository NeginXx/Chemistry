#pragma once
#include "main.h"
#include "Render.h"
#include "CoordinateSystem.h"

class PixelRectangle {
 public:
  PixelRectangle(const Point2D<size_t>& left_corner,
                 const Point2D<size_t>& right_corner,
                 const Color& color = {});
  void Draw(Render& render) const;

 private:
  Point2D<size_t> left_corner_;
  Point2D<size_t> right_corner_;
  Color color_;
};

class PixelVector {
 public:
  PixelVector(const Point2D<size_t>& origin,
              const Point2D<size_t>& end,
              const Color& color = {});
  void Normalize(float length);
  void Rotate(float angle);
  void Draw(Render& render) const;
  Point2D<size_t> GetOrigin() const;
  Point2D<size_t> GetEnd() const;

 private:
  Point2D<size_t> origin_;
  Point2D<size_t> end_;
  Color color_;
};

class Shape {
 public:
  virtual void Draw(const CoordinateSystem& coord_sys, Render& render) const = 0;
  Color GetColor() const;

 protected:
	Color color_;
};

class Line : public virtual Shape {
 public:
 	Line(const Point2D<float>& a, const Point2D<float>& b,
 		   const Color& color = {}, bool is_thick = false);
  Point2D<float> GetA() const;
  Point2D<float> GetB() const;
 	void Draw(const CoordinateSystem& coord_sys, Render& render) const override;

 protected:
 	Point2D<float> a_;
 	Point2D<float> b_;
  bool is_thick_;
};

class Rectangle : public virtual Shape {
 public:
 	Rectangle(const Point2D<float>& left_corner,
            const Point2D<float>& right_corner,
            const Color& color = {});
 	void Draw(const CoordinateSystem& coord_sys, Render& render) const override;
  Point2D<float> GetLeftCorner() const;
  Point2D<float> GetRightCorner() const;
  Point2D<float> GetCenter() const;
  float GetWidth() const;
  float GetHeight() const;

 protected:
  Point2D<float> left_corner_;
	Point2D<float> right_corner_;
};

class Vector : public virtual Shape {
 public:
  Vector(const Point2D<float>& origin,
         const Point2D<float>& coordinate,
         const Color& color = {});
  void Rotate(float angle);
  void Draw(const CoordinateSystem& coord_sys, Render& render) const  override;

 private:
  Point2D<float> origin_;
  Point2D<float> coordinate_;
};

class Circle : public virtual Shape {
 public:
  Circle(const Point2D<float>& center,
         float radius,
         const Color& color = {});
  void Draw(const CoordinateSystem& coord_sys, Render& render) const override;
  Point2D<float> GetCenter() const;
  float GetRadius() const;

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
  PhysicalShape(ShapeType type_, PhysicalShape* collision_partner);
  virtual ~PhysicalShape() = default;
  virtual void Move(float dt) = 0;
  const ShapeType type;
  PhysicalShape* collision_partner;
};

class PhysicalRectangle : public PhysicalShape, public Rectangle {
 public:
  PhysicalRectangle(const Point2D<float>& left_corner,
                    const Point2D<float>& right_corner,
                    const Vec2D<float>& velocity,
                    float mass,
                    const Color& color = {});
  PhysicalRectangle(const Point2D<float>& center,
                    float width,
                    float height,
                    const Vec2D<float>& velocity,
                    float mass,
                    const Color& color = {});
  void Move(float dt) override;
  Vec2D<float> GetVelocity() const;
  float GetMass() const;
  void SetVelocity(const Vec2D<float>& vec);

 protected:
  Vec2D<float> velocity_;
  float mass_;
};

class PhysicalCircle : public PhysicalShape, public Circle {
 public:
  PhysicalCircle(const Point2D<float>& center,
                 const Vec2D<float>& velocity,
                 float mass,
                 float radius,
                 const Color& color = {});
  void Move(float dt) override;
  Vec2D<float> GetVelocity() const;
  float GetMass() const;
  void SetVelocity(const Vec2D<float>& vec);

 private:
  Vec2D<float> velocity_;
  float mass_;
};

class PhysicalWall : public PhysicalShape, public Line {
 public:
  PhysicalWall(const Point2D<float>& a,
               const Point2D<float>& b,
               const Color& color = {},
               float temperature = 0);
  void Move(float dt) override;
  void IncreaseTemperature(float diff_temp);
  float GetTemperature() const;

 private:
  float temperature_;
};