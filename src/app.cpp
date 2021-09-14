#include <math.h>
#include "../include/main.h"
#include "../include/render.h"
#include "../include/shapes.h"
#include "../include/CoordinateSystem.h"

float ParabolaFunc(float x) {
  return x * x - 1;
}

float SinFunc(float x) {
  return sin(x);
}

void DrawEverything(Render& render) {
  static const Color kLightPurple = {171, 60, 255},
                     kLightGreen  = {107, 216, 79},
                     kLightPink   = {255, 153, 204},
                     kBlue        = {0, 0, 255},
                     kBlack       = {0, 0, 0},
                     kWhite       = {255, 255, 255};

  render.ChangeBackgroundColor(kBlue);

  Rectangle frame1(100, 100, 500, 300, kLightPink);
  frame1.Draw(render);
  Axes axes1({100, 250}, {600, 250},
  	         {350, 400}, {350, 100}, {8.0, 8.0}, kBlack);
  axes1.Draw(render);

  Rectangle frame2(150, 450, 1100, 350, kLightGreen);
  frame2.Draw(render);
  Axes axes2({150, 700}, {1250, 700},
             {500, 800}, {500, 450}, {15.0, 4.0}, kWhite);
  axes2.Draw(render);

  CoordinateSystem coord_sys1(&axes1);
  VectorInCoordinateSystem vec1({0, 0}, {4, 4}, kWhite);

  static float angle = -0.01;
  if (angle > 62.8) {
    angle = -0.01;
  }
  angle += 0.01;
  vec1.Rotate(angle);
  coord_sys1.DrawVector(vec1, render);

  CoordinateSystem coord_sys2(&axes2);
  VectorInCoordinateSystem vec2({1, 1}, {-1, -1}, kWhite);
  VectorInCoordinateSystem vec3({1, 1}, {0, 1}, kWhite);
  VectorInCoordinateSystem vec4({9, 1}, {-2.18, 0}, kBlack);
  vec2.Rotate(angle);
  vec3.Rotate(angle);
  vec4.Rotate(angle);
  coord_sys2.DrawVector(vec2, render);
  coord_sys2.DrawVector(vec3, render);
  coord_sys2.DrawVector(vec4, render);

  coord_sys1.DrawFunction(ParabolaFunc, render, kBlack);
  coord_sys2.DrawFunction(SinFunc, render);
}