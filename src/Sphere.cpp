#include "../include/Sphere.h"

Sphere::Sphere(float radius, const Color& color) {
  radius_ = radius;
  color_ = color;
}

void BoundVector(Vec3D<float>& vec) {
  #define BOUND(x)       \
    if (vec.x < 0.1) {   \
      vec.x = 0.1;       \
    } else if (vec.x > 255.1) { \
      vec.x = 255.1;     \
    }
  BOUND(x);
  BOUND(y);
  BOUND(z);
  #undef BOUND
}

void Sphere::Draw(const Vec3D<float>& light_pos,
                  const Vec3D<float>& light_color,
                  const Vec3D<float>& observer_pos,
                  const CoordinateSystem& coord_sys,
                  Render& render) const {
  Vec3D<float> sphere_color = {(float)color_.red,
                               (float)color_.green,
                               (float)color_.blue};
  sphere_color /= 255.0f;
  const Vec3D<float> ambient_color{0.1f, 0.1f, 0.1f};

  Point2D<size_t> left_corner  = coord_sys.ConvertCoordinate(Point2D<float>{-radius_, radius_});
  Point2D<size_t> right_corner = coord_sys.ConvertCoordinate(Point2D<float>{radius_, -radius_});

  for (size_t x_sizet = left_corner.x; x_sizet <= right_corner.x; ++x_sizet) {
    for (size_t y_sizet = left_corner.y; y_sizet <= right_corner.y; ++y_sizet) {

      Point2D<float> point_2d = coord_sys.ConvertCoordinate(Point2D<size_t>{x_sizet, y_sizet});
      Vec3D<float> point = {point_2d.x, point_2d.y, 0.0f};
      if (point.x * point.x + point.y * point.y > radius_ * radius_) { 
        continue; 
      }

      point.z = sqrtf(radius_ * radius_ - point.x * point.x - point.y * point.y);
      Vec3D<float> normal_vec   = Normalize(point);
      Vec3D<float> light_vec    = Normalize(light_pos - point);
      Vec3D<float> observer_vec = Normalize(observer_pos - point);

      float diffuse_val = DotProduct(normal_vec, light_vec);
      if (diffuse_val < 0.0f) {
          diffuse_val = 0.0f;
      }
      float specular = DotProduct(normal_vec * (2.0f * DotProduct(normal_vec, light_vec)) - light_vec,
                                  observer_vec);
      if (specular < 0.0f) {
        specular = 0.0f;
      }
      float specular_val = powf(specular, 10.0f);

      Vec3D<float> res_color = ambient_color * sphere_color +
                                 diffuse_val * sphere_color * light_color + 
                                specular_val * light_color;
      res_color *= 255.0f;
      BoundVector(res_color);
      typedef unsigned char BYTE;
      render.DrawPoint(Point2D<size_t>{x_sizet, y_sizet},
                       Color{(BYTE)res_color.x, (BYTE)res_color.y, (BYTE)res_color.z});

    }
  }
}