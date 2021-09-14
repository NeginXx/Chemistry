#include <stdio.h>
#include <stdlib.h>

class A {
public:
    virtual void pr() {
        printf("a\n");
    }    
};

class B : public A {
public:
    virtual void pr() override {
        printf("b\n");
    }
};

int main() {
	// так не работает как мы хотим
  A a{};
  B b{};
  A mas[2] = {a, b};
  mas[1].pr();

  // а так работает
  A a{};
  B b{};
  A* mas[2] = {&a, &b};
  mas[1]->pr();
}