#pragma once
#include <stddef.h>
#include <assert.h>

struct Color {
	unsigned char red = 0;
	unsigned char green = 0;					
	unsigned char blue = 0;
};

template <typename T>
struct Point2D {
	T x;
	T y;
};

Point2D<size_t> operator+(const Point2D<size_t>& lhs,
                          const Point2D<size_t>& rhs);

Point2D<float> operator+(const Point2D<float>& lhs,
                         const Point2D<float>& rhs);