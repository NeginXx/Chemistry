#include "main.h"

enum EventType {
  kQuit,
  kKeyboardClick,
  kMouseClick
};

union EventValue {
  uint8_t scancode;
  Point2D<size_t> coordinate;
};

struct Event {
  EventType type;
  EventValue value;
};

bool IsSomeEventInQueue(Event* event);