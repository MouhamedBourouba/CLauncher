#include "raylib/raylib.h"

const int fontSize = 24;
const Color fontColor = BLACK;
const char *inputLabel = "search: ";
const unsigned blinkingSpeed = 1000;

void drawTextInputBox(Rectangle boundingBox, const char *label) {
  static unsigned counter = 0;
  static bool isVisible;

  unsigned labelX = boundingBox.x + 16;
  unsigned labelY = (boundingBox.height - 24) / 2;
  DrawText(label, labelX, labelY, fontSize, fontColor);

  int endY = boundingBox.height;
  int endX = boundingBox.x + boundingBox.width;
  DrawLine(boundingBox.x, endY, endX, endY, BLACK);

  counter += GetFrameTime() * 1000;
  if (counter >= blinkingSpeed) {
    counter = 0;
    isVisible = !isVisible;
  }
  if (isVisible) {
    unsigned width = MeasureText(label, fontSize);
    DrawRectangle(labelX + width, labelY, 5, 24, BLACK);
  }
}

int main() {
  InitWindow(0, 0, "zig is awesome");
  SetWindowState(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
  SetTargetFPS(30);

  // 70 percent of screen width
  const int monitor = GetCurrentMonitor();
  const int monitorWidth = GetMonitorWidth(monitor);
  const int monitorHeight = GetMonitorHeight(monitor);
  const int windowWidth = monitorWidth * 5 / 10;
  const int windowHeight = 600;

  SetWindowSize(windowWidth, windowHeight);
  SetWindowPosition((monitorWidth - windowWidth) / 2,
                    (monitorHeight - windowHeight) / 2 - 50);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawTextInputBox({0, 0, (float)windowWidth, 50}, inputLabel);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
