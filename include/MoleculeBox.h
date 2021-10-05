#pragma once
#include "shapes.h"
#include "list.h"

class MoleculeBox {
 public:
  MoleculeBox(Point2D<float> left_corner,
              Point2D<float> right_corner,
              Color frame_color,
              Color background_color);
  ~MoleculeBox();
  void ProcessOneIteration(float dt);
  void Draw(CoordinateSystem& coord_sys, Render& render);
  template <typename ShapeType, class... Args>
  void AddShape(Args... args);

 private:
  List<PhysicalShape*> shapes_;
  Point2D<float> left_corner_;
  Point2D<float> right_corner_;
  Color frame_color_;
  Color background_color_;
};

template <typename ShapeType, class... Args>
void MoleculeBox::AddShape(Args... args) {
  ShapeType* shape = new ShapeType(args...);
  shapes_.PushFront(shape);
}