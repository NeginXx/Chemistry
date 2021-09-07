#include <assert.h>
#include <SDL2/SDL_ttf.h>
#include "../include/main.h"
#include "../include/graph.h"
#include <iostream>



float ParabolaFunc(float x) {
	return x * x - 1;
}

float SinFunc(float x) {
	return sin(x);
}

void DrawAxes(Window* win, Axes* axes) {
  Color color = axes->color;
  SDL_SetRenderDrawColor(win->renderer, color.red, color.green, color.blue, 0);
  GraphFrame* frame = axes->frame_ptr;

  // ------------------------------------------------------------------------------------
  // Drawing Ox

  size_t x1 = frame->x;
  size_t y1 = axes->center_y + frame->y;
  size_t x2 = frame->x + frame->width;
  size_t y2 = axes->center_y + frame->y;
  SDL_RenderDrawLine(win->renderer, (int)x1, (int)y1,
  	                                (int)x2, (int)y2);
  DrawText(win, "x", x2 - 13, y2 - 25);
  SDL_RenderDrawLine(win->renderer, (int)(x2 - 10), (int)(axes->center_y + frame->y - 5),
  	                                (int)(x2 - 10), (int)(axes->center_y + frame->y + 5));
  char num_str[10] = {};
  sprintf(num_str, "%lu\n", axes->max_x);
  size_t max_x_coord = x2 - 15; // relative to window
  DrawText(win, num_str, max_x_coord, axes->center_y + frame->y + 10);

  axes->x_1 = axes->center_x + (size_t)((float)(max_x_coord - frame->x - axes->center_x) / (float)axes->max_x);
  SDL_RenderDrawLine(win->renderer, (int)(axes->x_1 + frame->x), (int)(axes->center_y + frame->y - 5),
  	                                (int)(axes->x_1 + frame->x), (int)(axes->center_y + frame->y + 5));
  DrawText(win, "1", axes->x_1 + frame->x - 2, axes->center_y + frame->y + 10);

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  // ------------------------------------------------------------------------------------
  // Drawing Oy

  x1 = axes->center_x + frame->x;
  y1 = frame->y;
  x2 = axes->center_x + frame->x;
  y2 = frame->y + frame->height;
  SDL_RenderDrawLine(win->renderer, (int)x1, (int)y1,
  	                                (int)x2, (int)y2);
  DrawText(win, "y", x1 - 15, y1 + 3);
  SDL_RenderDrawLine(win->renderer, (int)(axes->center_x + frame->x + 5), (int)(frame->y + 10),
  	                                (int)(axes->center_x + frame->x - 5), (int)(frame->y + 10));
  sprintf(num_str, "%lu\n", axes->max_y);
  size_t max_y_coord = frame->y + 15; // relative to window
  DrawText(win, num_str, axes->center_x + frame->x + 10, frame->y);

  axes->y_1 = axes->center_y - (size_t)((float)(axes->center_y - (max_y_coord - frame->y)) / (float)axes->max_y);
  SDL_RenderDrawLine(win->renderer, (int)(axes->center_x + frame->x + 5), (int)(axes->y_1 + frame->y),
  	                                (int)(axes->center_x + frame->x - 5), (int)(axes->y_1 + frame->y));
  DrawText(win, "1", axes->center_x + frame->x - 18, axes->y_1 + frame->y - 8);

  // ------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------

  DrawText(win, "0", axes->center_x + frame->x - 18, axes->center_y + frame->y + 8);
}





  size_t x1 = frame->x;
  size_t y1 = axes->center_y + frame->y;
  size_t x2 = frame->x + frame->width;
  size_t y2 = axes->center_y + frame->y;
  SDL_RenderDrawLine(win->renderer, (int)x1, (int)y1,
  	                                (int)x2, (int)y2);
  DrawText(win, "x", x2 - 13, y2 - 25);
  SDL_RenderDrawLine(win->renderer, (int)(x2 - 10), (int)(axes->center_y + frame->y - 5),
  	                                (int)(x2 - 10), (int)(axes->center_y + frame->y + 5));
  char num_str[10] = {};
  sprintf(num_str, "%lu\n", axes->max_x);
  size_t max_x_coord = x2 - 15; // relative to window
  DrawText(win, num_str, max_x_coord, axes->center_y + frame->y + 10);

  axes->x_1 = axes->center_x + (size_t)((float)(max_x_coord - frame->x - axes->center_x) / (float)axes->max_x);
  SDL_RenderDrawLine(win->renderer, (int)(axes->x_1 + frame->x), (int)(axes->center_y + frame->y - 5),
  	                                (int)(axes->x_1 + frame->x), (int)(axes->center_y + frame->y + 5));
  DrawText(win, "1", axes->x_1 + frame->x - 2, axes->center_y + frame->y + 10);







void DrawGraphFrame(Window* win, GraphFrame* frame) {
  SDL_Rect rect = {(int)frame->x, (int)frame->y,
  	               (int)frame->width, (int)frame->height};
  Color color = frame->color;
  SDL_SetRenderDrawColor(win->renderer, color.red, color.green, color.blue, 0);
  SDL_RenderFillRect(win->renderer, &rect);
};

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