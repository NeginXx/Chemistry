#include "main.h"
#include "CoordinateSystem.h"

template <typename T>
struct Vec3D {
	T x;
	T y;
	T z;
	float Length() const {
	  return sqrtf(x * x + y * y + z * z);
	}
	void operator*=(T val) {
	  x *= val; y *= val; z *= val;
	}
	void operator/=(T val) {
	  x /= val; y /= val; z /= val;
	}
};

template <typename T>
Vec3D<T> operator/(const Vec3D<T>& v, T val) {
  return Vec3D<T>{v.x / val, v.y / val, v.z / val};
}

template <typename T>
Vec3D<T> Normalize(const Vec3D<T>& v) {
  return v / v.Length();
}

template <typename T>
Vec3D<T> operator+(const Vec3D<T>& v1, const Vec3D<T>& v2) {
  return Vec3D<T>{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

template <typename T>
Vec3D<T> operator-(const Vec3D<T>& v1, const Vec3D<T>& v2) {
  return Vec3D<T>{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

template <typename T>
Vec3D<T> operator*(const Vec3D<T>& v1, const Vec3D<T>& v2) {
  return Vec3D<T>{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

template <typename T>
Vec3D<T> operator*(const Vec3D<T>& v, T val) {
  return Vec3D<T>{v.x * val, v.y * val, v.z * val};
}

template <typename T>
Vec3D<T> operator*(T val, const Vec3D<T>& v) {
  return v * val;
}

template <typename T>
T DotProduct(const Vec3D<T>& v1, const Vec3D<T>& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

class Sphere {
 public:
 	Sphere(float radius_, const Color& color_);
 	void Draw(const Vec3D<float>& light_pos,
 			      const Vec3D<float>& light_color,
 						const Vec3D<float>& observer_pos,
 		        const CoordinateSystem& coord_sys,
 		        Render& render) const;
 private:
  float radius_;
  Color color_;
};