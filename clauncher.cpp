#include "raylib/raylib.h"
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <string>

const int fontSize = 24;
const Color fontColor = BLACK;
const char *inputLabel = "Run: ";
const unsigned blinkingSpeed = 400;
const unsigned cursorWidth = 1;

bool isCtrlHeld = false;

struct TextInput {

  TextInput(const Rectangle &rect, const char *inputLable)
      : boundingBox(rect), label(inputLable) {}

  void clear() {
    buff.clear();
    cursorPosX = 0;
  }

  void render() {
    unsigned labelX = boundingBox.x + 16;
    unsigned labelY = (boundingBox.height - 24) / 2;
    DrawText(label, labelX, labelY, fontSize, fontColor);

    int endY = boundingBox.height;
    int endX = boundingBox.x + boundingBox.width;
    DrawLine(boundingBox.x, endY, endX, endY, BLACK);

    unsigned labelWidth = MeasureText(label, fontSize);

    counter += GetFrameTime() * 1000.0;
    if (counter >= blinkingSpeed) {
      counter = 0;
      isVisible = !isVisible;
    }

    if (isVisible) {
      int spacing = fontSize / 10;
      int startX = labelX + labelWidth;
      int textWidth = MeasureText(buff.substr(0, cursorPosX).c_str(), fontSize);

      int X = startX + textWidth + 1;

      DrawRectangle(X, labelY, cursorWidth, fontSize, BLACK);
    }

    for (int key = GetCharPressed(); key != 0; key = GetCharPressed()) {
      char buffer[2] = {(char)key, '\0'};
      buff.insert(cursorPosX, buffer);
      cursorPosX++;
    }

    if (buff.length() > 0) {
      if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
        buff.erase(cursorPosX - 1, 1);
        cursorPosX--;
      }
      if (cursorPosX < buff.length() && IsKeyPressed(KEY_RIGHT)) {
        cursorPosX++;
      } else if (cursorPosX > 0 && IsKeyPressed(KEY_LEFT)) {
        cursorPosX--;
      }

      if (isCtrlHeld && IsKeyPressed(KEY_LEFT)) {

        if (buff.substr(0, cursorPosX).find(' ') != std::string::npos) {
          size_t currentIndex = cursorPosX;
          for (; buff.length() > currentIndex && buff[currentIndex] != ' ';
               currentIndex--)
            ;
          cursorPosX = currentIndex;
        } else cursorPosX = 0;
      }
    }

    DrawText(buff.c_str(), labelX + labelWidth, labelY, fontSize, fontColor);
  }

private:
  Rectangle boundingBox;
  const char *label;

  unsigned counter = 0;
  bool isVisible = true;

  std::string buff;

  unsigned cursorPosX = 0;
};

int main() {
  InitWindow(0, 0, "zig is awesome");
  SetWindowState(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
  SetTargetFPS(30);

  // 50 percent of screen width
  const int monitor = GetCurrentMonitor();
  const int monitorWidth = GetMonitorWidth(monitor);
  const int monitorHeight = GetMonitorHeight(monitor);
  const int windowWidth = monitorWidth * 5 / 10;
  const int windowHeight = 600;

  SetWindowSize(windowWidth, windowHeight);
  SetWindowPosition((monitorWidth - windowWidth) / 2,
                    (monitorHeight - windowHeight) / 2 - 50);

  auto textInput = TextInput({0, 0, (float)windowWidth, 50}, inputLabel);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (IsKeyPressed(KEY_LEFT_CONTROL))
      isCtrlHeld = true;
    if (IsKeyReleased(KEY_LEFT_CONTROL))
      isCtrlHeld = false;
    if (isCtrlHeld && IsKeyPressed(KEY_D)) {
      textInput.clear();
    }

    textInput.render();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
