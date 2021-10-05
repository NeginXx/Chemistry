#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "../include/main.h"
#include "../include/render.h"
#include "../include/window.h"
#include "../include/shapes.h"
#include "../include/CoordinateSystem.h"
#include "../include/list.h"
#include "../include/MoleculeBox.h"

struct Color kLightPurple = {171, 60, 255},
             kLightGreen  = {107, 216, 79},
             kLightPink   = {255, 153, 204},
             kBlue        = {0, 0, 255},
             kRed         = {255, 0, 0},
             kBlack       = {0, 0, 0},
             kWhite       = {255, 255, 255};

float ParabolaFunc(float x) {
  return x * x - 1;
}

float SinFunc(float x) {
  return sin(x);
}

bool SomeEventInQueue(SDL_Event* event_ptr) {
  return SDL_PollEvent(event_ptr);
}

void DrawEverything(CoordinateSystem& coord_sys,
                    MoleculeBox& box,
                    Render& render) {
  render.ChangeBackgroundColor(kWhite);
  box.ProcessOneIteration(0.001);
  box.Draw(coord_sys, render);
}

void RunAppMainCycle(CoordinateSystem& coord_sys,
                     MoleculeBox& box,
                     Window& window, Render& render) {
  clock_t t = clock();
  SDL_Event event = {};
  bool is_running = true;
  while (is_running) {
    DrawEverything(coord_sys, box, render);
    window.RenderPresent(render);
    // if ((double)(clock() - t) / CLOCKS_PER_SEC > 40.0) {
    //   break;
    // }
    // SDL_Delay(200);
    while (SomeEventInQueue(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = false;
        break;
      }
    }
  }
}

float RandomFloat(float min, float max) {
  int mmin = (int)(min * 10);
  int mmax = (int)(max * 10);
  return (float)(rand() % (100 * (mmax - mmin)) + mmin * 100) / 1000;
}

void RunApp(Window& window, Render& render) {
  size_t width = window.GetWidth();
  size_t height = window.GetHeight();
  float max_x = 50.0;
  float max_y = max_x * (float)height / (float)width;
  CoordinateSystem coord_sys({0, 0}, {width, height}, {-max_x, -max_y}, {max_x, max_y});
  MoleculeBox box({-(max_x * 0.8), max_y * 0.8}, {max_x * 0.8, -(max_y * 0.8)}, kRed, kWhite);

  // #define DEBUG
  #ifdef DEBUG
  Vec2D<float> crl1_velocity = {4.0, 8.0};
  Vec2D<float> crl2_velocity = {-4.0, -4.0};
  Vec2D<float> crl3_velocity = {-4.0, 4.0};
  Vec2D<float> rect1_velocity = {10.0, 0.0};
  Vec2D<float> rect2_velocity = {-10.0, 0.0};
  float m1 = 4.0;
  float m2 = 2.0;
  float factor = 2;
  box.AddShape<PhysicalCircle>(Point2D<float>(0.0, 0.0),  crl1_velocity * factor, m1, sqrtf(m1 / kPi), kBlue);
  box.AddShape<PhysicalCircle>(Point2D<float>(4.0, 4.0), crl2_velocity * factor, m2, sqrtf(m2 / kPi), kBlue);
  box.AddShape<PhysicalCircle>(Point2D<float>(3.0, -3.0), crl3_velocity * factor, 2.0, 0.8, kBlue);
  box.AddShape<PhysicalRectangle>(Point2D<float>(-5.0, 0.0), 3.0, 3.0, rect1_velocity * factor, 2.0, kBlue);
  box.AddShape<PhysicalRectangle>(Point2D<float>(5.0, 0.0), 2.0, 2.0, rect2_velocity * factor, 1.0, kBlue);

  #else
  float factor = 5;
  for (size_t i = 0; i < 30; ++i) {
    Point2D<float> center = {RandomFloat(-7.5, 7.5), RandomFloat(-4, 4)};
    Vec2D<float> velocity = {RandomFloat(-20, 20), RandomFloat(-20, 20)};
    float mass = RandomFloat(0.2, 2);
    float radius = sqrtf(mass / kPi) * 0.8;
    box.AddShape<PhysicalCircle>(center, velocity * factor, mass, radius, kBlue);
  }

  for (size_t i = 0; i < 10; ++i) {
    Point2D<float> center = {RandomFloat(-7.5, 7.5), RandomFloat(-4, 4)};
    float width = RandomFloat(0.4, 1.2);
    float height = RandomFloat(0.4, 1.2);
    Vec2D<float> velocity = {RandomFloat(-20, 20), RandomFloat(-20, 20)};
    float mass = RandomFloat(0.2, 2);
    box.AddShape<PhysicalRectangle>(center, width, height, velocity * factor, mass, kBlue);
  }
  #endif

  RunAppMainCycle(coord_sys, box, window, render);
}

// void DrawEverything(CoordinateSystem& coord_sys1,
//                     CoordinateSystem& coord_sys2,
//                     Vector* vectors, Render& render) {
//   render.ChangeBackgroundColor(kBlue);
//   coord_sys1.Draw(render, kLightPink);
//   coord_sys2.Draw(render, kWhite);

//   static float angle = -0.01;
//   if (angle > 62.8) {
//     angle = -0.001;
//   }
//   angle += 0.001;

//   for (size_t i = 0; i < 4; ++i) { vectors[i].Rotate(angle); }
//   vectors[0].Draw(coord_sys1, render);
//   for (size_t i = 1; i < 4; ++i) { vectors[i].Draw(coord_sys2, render); }
//   coord_sys1.DrawFunction(ParabolaFunc, render, kBlack);
//   coord_sys2.DrawFunction(SinFunc, render, kBlue);
// }

// void RunApp(Window& window, Render& render) {
//   CoordinateSystem coord_sys1({100, 100}, {600, 400},
//                               {-3.0, -4.0}, {8.0, 8.0}, kBlack);

//   CoordinateSystem coord_sys2({150, 450}, {1250, 800},
//                               {-3.0, -2.0}, {15.0, 4.0}, kLightGreen);
//   Vector vectors[] = {
//     { {0, 0}, {-3, 0}, kWhite },
//     { {1, 1}, {-1, -1}, kBlack },
//     { {1, 1}, {0, 1}, kBlack },
//     { {9, 1}, {-2.18, 0}, kBlack }
//   };

//   clock_t t = clock();
//   SDL_Event event = {};
//   bool is_running = true;
//   while (is_running) {
//     DrawEverything(coord_sys1, coord_sys2, vectors, render);
//     window.RenderPresent(render);
//     if ((double)(clock() - t) / CLOCKS_PER_SEC > 40.0) {
//       break;
//     }
//     SDL_Delay(50);
//     while (SomeEventInQueue(&event)) {
//       if (event.type == SDL_QUIT) {
//         is_running = false;
//         break;
//       }
//     }
//   }
// }