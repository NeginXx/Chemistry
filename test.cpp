#include <iostream>

struct A {
  virtual void operator()(int a) {
    std::cout << a << '\n';
  }
};

struct B : public A {
  void operator()(int a) override {
    std::cout << a + 1 << '\n';
  }
};

int main() {
  unsigned char c = 255;
  printf("%d\n", c + (unsigned char)1 < c);
}