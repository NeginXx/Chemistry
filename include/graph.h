
void DrawGraphFrame(Window* win, GraphFrame* frame);
void DrawCoordSystem(Window* win, CoordSystem* axes);
void DrawFunction(Window* win, CoordSystem* axes, float (*func)(float));
void DrawVector(Window* win, Vector* vec);
void RotateVector(Window* win, Vector* vec, float angle);