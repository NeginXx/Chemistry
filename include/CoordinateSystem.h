#pragma once

class VectorInCoordinateSystem {
 public:
 	VectorInCoordinateSystem(Point2D<float> origin,
 		                       Point2D<float> coordinate,
 		                       Color color = {});
 	void Rotate(float angle);
 	friend class CoordinateSystem;

 private:
 	Point2D<float> origin_;
	Point2D<float> coordinate_;
	Color color_;
};

class CoordinateSystem {
 public:
 	CoordinateSystem(Axes* axes);
 	void DrawVector(VectorInCoordinateSystem& vector, Render& render);
 	void DrawFunction(float (*func)(float), Render& render, Color color = {});

 private:
 	Point2D<size_t> ConvertCoordinate(Point2D<float> a);
 	Axes* axes_;
};