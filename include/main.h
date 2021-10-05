#pragma once
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "StackTrace.h"

const float kPi = 3.1416;

struct Color {
	unsigned char red = 0;
	unsigned char green = 0;					
	unsigned char blue = 0;
};

template<typename T>
struct Vec2D;

template <typename T>
struct Point2D {
	T x;
	T y;
	Point2D() = default;
	Point2D(T x_, T y_) {
		x = x_;
		y = y_;
	}
	Point2D(Vec2D<T> v) {
		x = v.x;
		y = v.y;
	}
};

template <typename T>
struct Vec2D {
	T x;
	T y;
	Vec2D(T x_, T y_) {
		x = x_;
		y = y_;
	}
	Vec2D(Point2D<T> a) {
		x = a.x;
		y = a.y;
	}
	void Rotate(float angle) {
  	float temp = x;
  	x = x * cosf(angle) - y * sinf(angle);
  	y = temp * sinf(angle) + y * cosf(angle);
	}
	T SquaredLength() {
		return pow(x, 2) + pow(y, 2);
	}
	T Length() {
		return sqrt(this->SquaredLength());
	}
	void operator=(Point2D<T> a) {
		x = a.x;
		y = a.y;
	}
};

template <typename T>
Point2D<T> operator+(const Point2D<T>& lhs,
                     const Point2D<T>& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T>
Point2D<T> operator-(const Point2D<T>& lhs,
                     const Point2D<T>& rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T>
Vec2D<T> operator+(const Vec2D<T>& lhs,
                   const Vec2D<T>& rhs) {
  Vec2D<T> v(lhs.x + rhs.x, lhs.y + rhs.y);
  return v;
}

template <typename T>
Vec2D<T> operator-(const Vec2D<T>& lhs,
                   const Vec2D<T>& rhs) {
  Vec2D<T> v(lhs.x - rhs.x, lhs.y - rhs.y);
  return v;
}

template <typename T>
T operator*(const Vec2D<T>& lhs,
            const Vec2D<T>& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T>
Vec2D<T> operator*(const Vec2D<T>& v,
                   T val) {
  Vec2D<T> ret(v.x * val, v.y * val);
  return ret;
}

template <typename T>
Vec2D<T> operator/(const Vec2D<T>& v,
                   T val) {
  Vec2D<T> ret(v.x / val, v.y / val);
  return ret;
}

template <typename T>
Point2D<T> operator/(const Point2D<T>& p,
                     T val) {
  Point2D<T> ret(p.x / val, p.y / val);
  return ret;
}