#include <iostream>
#include <vector>
#include <list>
#include "include/list.h"

class Up {
 public:
  virtual void Print() {
    printf("Up\n");
  }
  int a_;
};

class Left : public virtual Up {
 public:
  // virtual void Print() override {
  //   printf("Left\n");
  // }
  int b_;
};

class Right : public virtual Up {
 public:
  virtual void Print() override {
    printf("Right\n");
  }
  int b_;
};

class Down : public Left, public Right {
 public:
  virtual void Print() override {
    printf("Down\n");
  }
  int b_;
};

int main() {
  List<int> l = {1, 2, 3, 4};
  for (auto elem : l) {
    printf("%d\n", elem);
  }
  // Up up;
  // Left left;
  // Right right;
  // Down down;
  // // Up* arr[4] = {&up, &left, &right, &down};
  // // for (size_t i = 0; i < 4; ++i) {
  // //   arr[i]->Print();
  // // }
  // Left* down_ptr = &down;
  // down_ptr->Print();
}