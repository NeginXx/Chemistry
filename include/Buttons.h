#include <vector>
#include "main.h"
#include "Shapes.h"

class AbstractFunctor {
 public:
  virtual void operator()() = 0;
};

class MouseButton {
 public:
 	MouseButton(const Point2D<float>& left_corner,
 	            const Point2D<float>& right_corner,
 	            const Color& color,
 	            AbstractFunctor* process_func);
 	bool IsPressed(const Point2D<float>& click_point) const;
 	void Process() const;
 	void Draw(const CoordinateSystem& coord_sys,
 		        Render& render) const;

 private:
  Rectangle rect_;
 	AbstractFunctor* process_func_;
};

class KeyboardButton {
 public:
  KeyboardButton(uint8_t scancode,
	               AbstractFunctor* process_func);
 	bool IsPressed(uint8_t scancode) const;
 	void Process() const;

 private:
 	uint8_t scancode_;
 	AbstractFunctor* process_func_;
};

class ButtonManager {
 public:
  void ProcessMouseClick(Point2D<float> coordinate) const;
 	void ProcessKeyboardClick(uint8_t scancode) const;
 	void DrawButtons(const CoordinateSystem& coord_sys,
 		               Render& render) const;

 	template <class ButtonType, class... Args>
 	void AddButton(const Args&... args) {
 		ButtonType button(args...);
 		if constexpr (std::is_same<ButtonType, MouseButton>::value) {
 			mouse_buttons_.push_back(button);
 		} else {
 			keyboard_buttons_.push_back(button);
 		}
 	}

 private:
 	std::vector<MouseButton> mouse_buttons_;
 	std::vector<KeyboardButton> keyboard_buttons_;
};