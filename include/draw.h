#pragma once

class Window;
struct Color;

void ChangeWindowColor(Window* win, Color color);
void DrawFrame(Window* win, double left_x, double left_y,
	             double right_x, double right_y, Color color = {});
void RenderEverything(Window* win);