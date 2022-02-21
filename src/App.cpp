#include <math.h>
#include <time.h>
#include "../include/main.h"
#include "../include/Render.h"
#include "../include/Window.h"
#include "../include/Shapes.h"
#include "../include/CoordinateSystem.h"
#include "../include/List.h"
#include "../include/MoleculeBox.h"
#include "../include/Events.h"
#include "../include/Buttons.h"
#include "../include/Sphere.h"

const struct Color kLightPurple = {171, 60, 255},
                   kLightGreen  = {107, 216, 79},
                   kLightPink   = {255, 153, 204},
                   kPink        = {226, 102, 245},
                   kBlue        = {0, 0, 255},
                   kRed         = {255, 0, 0},
                   kBlack       = {0, 0, 0},
                   kWhite       = {255, 255, 255};

Color kCircleColor = kPink;
Color kRectangleColor = kBlack;

// struct PlotPoint {
//   float val = 0;
//   float dt = 0;
// };

// class DynamicPlot {
//  public:
//   DynamicPlot() = delete;
//   DynamicPlot(CoordinateSystem* coord_sys) {
//     coord_sys_ = coord_sys;
//   }
//   void AddPoint(float val, float dt) {
//     points_.PushFront({val, dt});
//     if (points_.GetSize() > 100) {
//       points_.PopBack();
//     }
//   }
//   void Draw(Render& render, const char* x_str, const char* y_str) {
//     if (points_.GetSize() <= 1) {
//       return;
//     }
//     coord_sys_->Draw(render, x_str, y_str);
//     auto prev = points_.end();
//     --prev;
//     Line line({0.0f, 0.0f}, {0.0f, 0.0f});
//     float cur_dt = 0;
//     for (auto point = prev.Prev(); point != points_.begin(); --point) {
//       cur_dt += (*prev).dt;
//       line.SetA(Point2D<float>{cur_dt, (*prev).val});
//       line.SetB(Point2D<float>{(*point).dt + cur_dt, (*point).val});
//       // printf("a.x = %f, a.y = %f\n", cur_dt, (*prev).val);
//       // printf("b.x = %f, b.y = %f\n", (*point).dt + cur_dt, (*point).val);
//       line.Draw(*coord_sys_, render);
//       prev = point;
//     }
//   }

//  private:
//   CoordinateSystem* coord_sys_;
//   List<PlotPoint> points_;
// };

class IncreaseBoxWallsTemperatureFunctor : public AbstractFunctor {
 public:
  IncreaseBoxWallsTemperatureFunctor(MoleculeBox* box) {
    box_ = box;
  }
  void operator()() override {
    box_->IncreaseWallsTemperature(10.0);
  }

 private:
  MoleculeBox* box_;
};

class DecreaseBoxWallsTemperatureFunctor : public AbstractFunctor {
 public:
  DecreaseBoxWallsTemperatureFunctor(MoleculeBox* box) {
    box_ = box;
  }
  void operator()() override {
    box_->IncreaseWallsTemperature(-10.0);
  }

 private:
  MoleculeBox* box_;
};

float RandomFloat(float min, float max) {
  int mmin = (int)(min * 10);
  int mmax = (int)(max * 10);
  return (float)(rand() % (100 * (mmax - mmin)) + mmin * 100) / 1000;
}

void ProcessOneFrame(const CoordinateSystem& coord_sys,
                     MoleculeBox& box,
                     const ButtonManager& manager,
                     Render& render) {
  const float frame_time = 0.001f;
  render.ChangeBackgroundColor(kWhite);
  box.ProcessOneIteration(frame_time);
  box.Draw(coord_sys, render);
  manager.DrawButtons(coord_sys, render);
}

void RunAppMode0(const Window& window, Render& render) {
  // Creating coordinate system
  // ------------------------------------------------------------------------------------
  const size_t width = window.GetWidth();
  const size_t height = window.GetHeight();
  const float max_x = 50.0f;
  const float max_y = max_x * (float)height / (float)width;
  CoordinateSystem coord_sys({0, 0}, {width, height}, {-max_x, -max_y}, {max_x, max_y});
  // ------------------------------------------------------------------------------------


  // Creating molecule box and shapes
  // ------------------------------------------------------------------------------------
  const Point2D<float> box_l_c = {-max_x, max_y * 0.9f};
  const Point2D<float> box_r_c = {max_x * 0.3f, -max_y * 0.7f};
  MoleculeBox box(box_l_c, box_r_c, kRed, kWhite);

  float factor = 5;
  for (size_t i = 0; i < 10; ++i) {
    Point2D<float> center = {RandomFloat(box_l_c.x + 1, 0), RandomFloat(0, box_l_c.y - 1)};
    Vec2D<float> velocity = {RandomFloat(-3, 3), RandomFloat(-3, 3)};
    float mass = RandomFloat(5, 10);
    float radius = sqrtf(mass / kPi);
    box.AddShape<PhysicalCircle>(center, velocity * factor, mass, radius, kCircleColor);
  }

  for (size_t i = 0; i < 30; ++i) {
    Point2D<float> center = {RandomFloat(box_l_c.x + 1, 0), RandomFloat(0, box_l_c.y - 1)};
    Vec2D<float> velocity = {RandomFloat(-3, 3), RandomFloat(-3, 3)};
    float mass = RandomFloat(5, 10);
    float width = sqrtf(mass);
    float height = sqrtf(mass);
    box.AddShape<PhysicalRectangle>(center, width, height, velocity * factor, mass, kRectangleColor);
  }
  // ------------------------------------------------------------------------------------


  // Creating button manager and buttons
  // ------------------------------------------------------------------------------------
  const float button_l_x = box_r_c.x + 10.0f;
  const float button_r_x = button_l_x + 15.0f;
  const float button_l_y = box_l_c.y;
  const float button_r_y = button_l_y - 10.0f;
  ButtonManager button_manager;
  IncreaseBoxWallsTemperatureFunctor inc_functor(&box);
  DecreaseBoxWallsTemperatureFunctor dec_functor(&box);
  button_manager.AddButton<MouseButton>(Point2D<float>{button_l_x, button_l_y},
                                        Point2D<float>{button_r_x, button_r_y},
                                        kRed,
                                        &inc_functor);

  const float second_button_r_y = button_r_y - 12.0f;
  button_manager.AddButton<MouseButton>(Point2D<float>{button_l_x, button_r_y - 2.0f},
                                        Point2D<float>{button_r_x, second_button_r_y},
                                        kBlue,
                                        &dec_functor);
  // ------------------------------------------------------------------------------------


  // // Creating plot
  // // ------------------------------------------------------------------------------------
  // CoordinateSystem temp(coord_sys.ConvertCoordinate(Point2D<float>{box_r_c.x + 3.0f, second_button_r_y - 1.0f}),
  //                       coord_sys.ConvertCoordinate(Point2D<float>{max_x, box_r_c.y}),
  //                       {-0.001f, -0.001f}, {0.1f, 10000.0f}, kBlue);
  // DynamicPlot plot(&temp);
  // // ------------------------------------------------------------------------------------


  clock_t t = clock();
  Event event = {};
  bool is_running = true;
  while (is_running) {
    ProcessOneFrame(coord_sys, box, button_manager, render);
    window.RenderPresent(render);
    // SDL_Delay(200);
    while (IsSomeEventInQueue(&event)) {
      if (event.type == kQuit) {
        is_running = false;
        break;
      }
      if (event.type == kMouseClick) {
        button_manager.ProcessMouseClick(coord_sys.ConvertCoordinate(event.value.coordinate));
      }
    }
  }
}

void RunAppMode1(const Window& window, Render& render) {
  // Creating coordinate system
  // ------------------------------------------------------------------------------------
  const size_t width = window.GetWidth();
  const size_t height = window.GetHeight();
  const float max_x = 50.0f;
  const float max_y = max_x * (float)height / (float)width;
  CoordinateSystem coord_sys({0, 0}, {width, height}, {-max_x, -max_y}, {max_x, max_y});
  // ------------------------------------------------------------------------------------

  float radius = 30.0f;
  Sphere sphere(radius, kBlue);
  float light_x = -2.0f * radius;
  float light_y = 1.0f * radius;
  float light_z = 2.0f * radius;
  Vec2D<float> vec = {light_x, light_z};
  Event event = {};
  bool is_running = true;
  while (is_running) {
    vec.Rotate(0.05);
    sphere.Draw({vec.x, light_y, vec.y}, {1.0f, 1.0f, 1.0f},
                {0.0f, 0.0f, 5.0f * radius}, coord_sys, render);
    window.RenderPresent(render);
    // SDL_Delay(200);
    while (IsSomeEventInQueue(&event)) {
      if (event.type == kQuit) {
        is_running = false;
        break;
      }
    }
  }
}

void RunApp(const Window& window, Render& render, int mode) {
  if (mode == 0) {
    RunAppMode0(window, render);
  } else if (mode == 1) {
    RunAppMode1(window, render);
  } else {
    assert(!"Invalid argument passed to the program");
  }
}

// float ParabolaFunc(float x) {
//   return x * x - 1;
// }

// float SinFunc(float x) {
//   return sin(x);
// }

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