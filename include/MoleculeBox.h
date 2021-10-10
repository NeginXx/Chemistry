#pragma once
#include "Shapes.h"
#include "List.h"

class MoleculeBox {
 public:
  MoleculeBox(const Point2D<float>& left_corner,
              const Point2D<float>& right_corner,
              const Color& frame_color,
              const Color& background_color);
  ~MoleculeBox();
  void ProcessOneIteration(float dt);
  void Draw(const CoordinateSystem& coord_sys, Render& render) const;
  template <typename ShapeType, typename... Args>
  void AddShape(const Args&... args);
  bool IsInBound(PhysicalShape* shape);
  void IncreaseWallsTemperature(float diff_temp);

 private:
  List<PhysicalShape*> shapes_;
  Point2D<float> left_corner_;
  Point2D<float> right_corner_;
  Color frame_color_;
  Color background_color_;
};

template <typename ShapeType, typename... Args>
void MoleculeBox::AddShape(const Args&... args) {
  ShapeType* shape = new ShapeType(args...);
  shapes_.PushFront(shape);
}