class CoordinateSystem {
 private:
		class Vector {
		 public:
		 	Vector(Point2D<float>& origin,
	 		       Point2D<float>& end,
	 		       Color color = {}) {
	 		  origin_ = origin;
	 		  end_ = end;
	 		  color = color_;
		 	}

		 	Rotate(float angle) {
		 	  x_ = x_ * cos(angle) - y_ * sin(angle);
	      y_ = x_ * sin(angle) + y_ * cos(angle);	
		 	}

		 private:
			Point2D<float>& origin,
	 		Point2D<float>& end
		};

 public:
 	CoordinateSystem(Point2D<size_t> left_up_corner,
 									 Point2D<size_t> right_down_corner,
 									 Point2D<float> max_coord,
 									 Color color = {}) {

 	}

 	DrawVector(Point2D<float>& origin,
 		         Point2D<float>& end,
 		         Color color = {}) {
 		
 	}

 private:
 	Axes axes;

 	int ConvertXCoord(Axes* axes, float x) {
		return (int)(x * (float)(axes->x_1 - axes->center_x)) + axes->frame_ptr->x + axes->center_x;
	}

	int ConvertYCoord(Axes* axes, float y) {
		return axes->frame_ptr->y + axes->center_y - (int)(y * (float)(axes->center_y - axes->y_1));
	}
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
 	Vector x_axis_;
 	Vector y_axis_;
 	Point2D<float> max_coord_;
	Point2D<size_t> point_1_1_;
};