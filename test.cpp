#include <iostream>

// class A {
//  public:
// 	virtual void Print() {
// 		printf("A\n");
// 	}
// };

// class B : public A {
//  public:
//  	void Print() override {
//  		printf("B\n");
//  	}
// };

// class C : public B {
//  public:
//  	void Print() override {
//  		printf("C\n");
//  	}
// };

// int main() {
// 	A a;
// 	B b;
// 	C c;
// 	A* ptr = &c;
// 	ptr->Print();
// }

int cnt = 0;

class Vector {
 public:
  Vector() {
  	printf("%d\n", cnt++);
  	x_ = 1;
  	y_ = 2;
  }
 private:
 	int x_;
 	int y_;
};

class Axes {
 public:
 	Axes(Vector& x_axis, Vector& y_axis) {
 		x_axis_ = x_axis;
 		y_axis_ = y_axis;
 	}

 private:
 	Vector x_axis_;
 	Vector y_axis_;
};

int main() {
	Vector v1;
	Vector v2;
	Axes axes(v1, v2);
}