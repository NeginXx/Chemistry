#include "../include/Buttons.h"

MouseButton::MouseButton(const Point2D<float>& left_corner,
 	                       const Point2D<float>& right_corner,
 	                       const Color& color,
 	                       AbstractFunctor* process_func)
 	: rect_(left_corner, right_corner, color),
 	  process_func_(process_func) {}

bool MouseButton::IsPressed(const Point2D<float>& click_point) const {
	Point2D<float> l_c = rect_.GetLeftCorner();
	Point2D<float> r_c = rect_.GetRightCorner();
	return l_c.x < click_point.x && click_point.x < r_c.x &&
	       r_c.y < click_point.y && click_point.y < l_c.y;
}

void MouseButton::Process() const {
 	(*process_func_)();
}

void MouseButton::Draw(const CoordinateSystem& coord_sys,
 		                   Render& render) const {
	rect_.Draw(coord_sys, render);
}




KeyboardButton::KeyboardButton(uint8_t scancode,
	                             AbstractFunctor* process_func)
  : scancode_(scancode), process_func_(process_func) {}

bool KeyboardButton::IsPressed(uint8_t scancode) const {
	return scancode_ == scancode;
}

void KeyboardButton::Process() const {
 	(*process_func_)();
}




void ButtonManager::ProcessMouseClick(Point2D<float> coordinate) const {
	for (auto button : mouse_buttons_) {
		if (button.IsPressed(coordinate)) {
			button.Process();
			break;
		}
	}
}

void ButtonManager::ProcessKeyboardClick(uint8_t scancode) const {
	for (auto button : keyboard_buttons_) {
		if (button.IsPressed(scancode)) {
			button.Process();
			break;
		}
	}
}

void ButtonManager::DrawButtons(const CoordinateSystem& coord_sys,
 		                            Render& render) const {
	for (auto button : mouse_buttons_) {
		button.Draw(coord_sys, render);
	}
}