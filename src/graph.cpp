#include <assert.h>
#include <SDL2/SDL_ttf.h>
#include "../include/main.h"
#include "../include/graph.h"
#include <iostream>

// takes coord related to axes, returns coord related to window
int ConvertXCoord(Axes* axes, float x) {
	return (int)(x * (float)(axes->x_1 - axes->center_x)) + axes->frame_ptr->x + axes->center_x;
}

int ConvertYCoord(Axes* axes, float y) {
	return axes->frame_ptr->y + axes->center_y - (int)(y * (float)(axes->center_y - axes->y_1));
}

void DrawPointInAxes(Window* win, Axes* axes, float x, float y) {
	SDL_RenderDrawPoint(win->renderer, ConvertXCoord(axes, x), ConvertYCoord(axes, y));
}

void DrawLineInAxes(Window* win, Axes* axes, float x1, float y1,
	                                           float x2, float y2) {
	SDL_RenderDrawLine(win->renderer, ConvertXCoord(axes, x1), ConvertYCoord(axes, y1),
		                                ConvertXCoord(axes, x2), ConvertYCoord(axes, y2));
}

void DrawFunction(Window* win, Axes* axes, float (*func)(float)) {
	float prev_x = -(float)axes->center_x / (float)(axes->x_1 - axes->center_x);
	float prev_y = func(prev_x);
	DrawPointInAxes(win, axes, prev_x, prev_y);
	float d_x = (float)axes->max_x / 50;
	for (float x = prev_x + d_x; x < (float)axes->max_x; x += d_x) {
		float y = func(x);
		if (abs(y) <= (float)axes->max_y) {
		  DrawPointInAxes(win, axes, x, y);
		  DrawLineInAxes(win, axes, x, y, prev_x, prev_y);
		}
		prev_x = x;
		prev_y = y;
	}
}

int BoundedX(GraphFrame* frame, int x) {
	if (x > frame->x + frame->width) {
		return frame->x + frame->width;
	}
	if (x < frame->x) {
		return frame->x;
	}
	return x;
}

int BoundedY(GraphFrame* frame, int y) {
	if (y > frame->y + frame->height) {
		return frame->y + frame->height;
	}
	if (y < frame->y) {
		return frame->y;
	}
	return y;
}

int ConvertLenX(Axes* axes, float len_x) {
	return (int)(len_x * (float)(axes->x_1 - axes->center_x));
}

int ConvertLenY(Axes* axes, float len_y) {
	return (int)(len_y * (float)(axes->center_y - axes->y_1));
}

void DrawVector(Window* win, Vector* vec) {
	Axes* axes = vec->axes_ptr;
	GraphFrame* frame = axes->frame_ptr;
  Color color = vec->color;
  SDL_SetRenderDrawColor(win->renderer, color.red, color.green, color.blue, 0);
  // these coordinates are related to window
  int x = ConvertXCoord(axes, vec->x + vec->origin_x);
  int y = ConvertYCoord(axes, vec->y + vec->origin_y);
  int origin_x = ConvertXCoord(axes, vec->origin_x);
  int origin_y = ConvertYCoord(axes, vec->origin_y);
  x = BoundedX(frame, x);
  y = BoundedY(frame, y);
  origin_x = BoundedX(frame, origin_x);
  origin_y = BoundedY(frame, origin_y);
  SDL_RenderDrawLine(win->renderer, x, y, origin_x, origin_y);

  float a = 30.0 / 180.0 * M_PI; // angle between arrow and vector
  float arrow_len = 1.0;
  float vec_len = sqrt(vec->x * vec->x + vec->y * vec->y);
  float arrow_x = arrow_len / vec_len * (vec->y * sin(a) - vec->x * cos(a));
  float arrow_y = arrow_len / vec_len * (-vec->y * cos(a) - vec->x * sin(a));
  float xx1 = x + ConvertLenX(axes, arrow_x);
  float yy1 = y - ConvertLenY(axes, arrow_y);
  float actual_arrow_len = sqrt(pow(xx1 - x, 2) + pow(yy1 - y, 2));
  float actual_vector_len = sqrt(pow(x - origin_x, 2) + pow(y - origin_y, 2));
  float needed_actual_arrow_len = actual_vector_len / 5;
  float k = actual_arrow_len / needed_actual_arrow_len;
  SDL_RenderDrawLine(win->renderer, x, y, x + ConvertLenX(axes, arrow_x) / k,
  	                                      y - ConvertLenY(axes, arrow_y) / k);

  arrow_x = arrow_len / vec_len * (vec->y * sin(a) + vec->x * cos(a));
  arrow_y = arrow_len / vec_len * (-vec->y * cos(a) + vec->x * sin(a));
  xx1 = x - ConvertLenX(axes, arrow_x);
  yy1 = y - ConvertLenY(axes, arrow_y);
  actual_arrow_len = sqrt(pow(xx1 - x, 2) + pow(yy1 - y, 2));
  k = actual_arrow_len / needed_actual_arrow_len;
  SDL_RenderDrawLine(win->renderer, x, y, x - ConvertLenX(axes, arrow_x) / k,
  	                                      y - ConvertLenY(axes, arrow_y) / k);
}

void DrawVector(Window* win, Vector* vec) {	
 private:
	float x_ = 0;
	float y_ = 0;
	float origin_x_ = 0;
	float origin_y_ = 0;
	Color color_;
}