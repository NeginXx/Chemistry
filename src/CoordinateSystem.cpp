class CoordSystem {
 private:
	class Vector {
	 public:
	 	Vector(float x, float y, float origin_x, float origin_y,
	 		     Color color = {}) {
	 		x_ = x;
	 		y_ = y;
	 		origin_x = origin_x_;
	 		origin_y = origin_y_;
	 		color_ = color;
	 	}

	 	Rotate(float angle) {
	 	  x_ = x_ * cos(angle) - y_ * sin(angle);
      y_ = x_ * sin(angle) + y_ * cos(angle);	
	 	}

	 private:
		float x_ = 0;
		float y_ = 0;
		float origin_x_ = 0;
		float origin_y_ = 0;
		Color color_;
	};
	size_t max_x = 0;
	size_t max_y = 0;
	// coordinates are relative to frame
	size_t center_x = 0;
	size_t center_y = 0;
	// coordinates of 1, relative to frame
	size_t x_1 = 0;
	size_t y_1 = 0;
	Color color;
};