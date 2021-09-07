class Shape {
 public:
 	Shape() {
 		next_ = list_;
 		list_ = this;
 	}

 	Shape* GetList() {
 		return list_;
 	}

 	virtual void Draw();

 protected:
	Color color_;

 private:
	static Shape* list_;
	Shape* next_;
};

class Line : public Shape {
 public:
 	Line(Point2D<size_t>& a, Point2D<size_t>& b,
 		   Color color = {}) {
 		a_ = a;
 		b_ = b;
 		color_ = color;
 	}

 	void Draw() override {
 		Window::DrawLine(a_, b_, color_);
 	}

 private:
 	Point2D<size_t> a_;
 	Point2D<size_t> b_;
};

class Rectangle : public Shape {
 public:
 	Rectangle(size_t x, size_t y, size_t width,
 		        size_t height, Color color = {}) {
 		left_corner_ = {x, y};
 		width_ = width;
 		height_ = height;
 		color_ = color;
 	}

 	void Draw() override {
 		Window::DrawRectangle(left_corner_, width_,
 			                    height_, color_);
 	}

 private:
	Point2D<size_t> left_corner_;
	size_t width_ = 0;
	size_t height_ = 0;
};

class Vector : public Shape {
 public:
 	Vector(Point2D<size_t>& origin,
 		     Point2D<size_t>& end,
 		     Color color = {}) {
 		origin_ = origin;
 		end_ = end;
 		color = color_;
 	}

 	void Rotate(float angle) {
 	  end_.x = end_.x * cos(angle) - end_.y * sin(angle);
    end_.y = end_.x * sin(angle) + end_.y * cos(angle);
 	}

 	void Draw() override {
 		// vec_end - end of auxiliary vector
 		Point2D<size_t> vec_end = { end_.x - (end_.x - origin_.x) / 5,
 		                            end_.y + (origin_.y - end.y) / 5 };
 		Vector vec(origin_, vec_end, color_);
 		float angle = 30.0 / 180.0 * M_PI;
 		vec.Rotate(angle);

 		Line arrow_line1(origin_, vec.GetEnd(), color_);
 		vec.Rotate(-angle * 2);
 		Line arrow_line2_(origin_, vec.GetEnd(), color_);

 		arrow_line1_.Draw();
 		arrow_line2_.Draw();
 		Line main_line(origin_, end_, color_);
 		main_line_.Draw();
 	}

 	Point2D<size_t> GetOrigin() {
 		return origin_;
 	}

 	Point2D<size_t> GetEnd() {
 		return end_;
 	}

 private:
 	Point2D<size_t> origin_;
  Point2D<size_t> end_;
};

class Axes : public Shape {
 public:
 	Axes(Point2D<size_t> x1,
 	     Point2D<size_t> y1,
 	     Point2D<size_t> x2,
 	     Point2D<size_t> y2,
 	     Point2D<float>& max_coord,
 	     Color color = {}) : x_axis_(x1, y1, color),
 	                         y_axis_(x2, y2, color) {
 		max_coord_ = max_coord;
 	}

 	void Draw() override {
 		x_axis_.Draw();
 		y_axis_.Draw();
 	}

 private:
  Point2D<size_t> x1_;
 	Point2D<size_t> y1_;
 	Point2D<size_t> x2_;
 	Point2D<size_t> y2_;
 	Point2D<float> max_coord_;
	Point2D<size_t> point_1_1_;
};