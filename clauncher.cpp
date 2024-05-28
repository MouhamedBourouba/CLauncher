#include "raylib/raylib.h"
#include <string>
#include <vector>

const int fontSize = 24;
const Color fontColor = BLACK;
const char *inputLabel = "Run: ";
const unsigned blinkingSpeed = 350;
const unsigned cursorWidth = 1;
const Color itemsSeperatorColor = GRAY;

Color seperatorColor = BLACK;
bool isCtrlHeld = false;

//  TODO: add highliting
struct TextInput {
  using InputCallback = void (*)(const std::string &);

  TextInput(const Rectangle &rect, const char *inputLable)
      : boundingBox(rect), label(inputLable) {}

  void clear() {
    inputBuffer.clear();
    cursorPosX = 0;
  }

  void setCallback(InputCallback cb) { callback = cb; }

  const std::string &input() { return inputBuffer; }

  void render() {
    DrawText(label, boundingBox.x, labelY, fontSize, fontColor);
    handelInput();
    handelShortCuts();
    DrawText(inputBuffer.c_str(), boundingBox.x + labelWidth, labelY, fontSize,
             fontColor);
    drawCursor();
  }

  Rectangle getBoundingBox() { return boundingBox; }

private:
  const char *label;

  std::string inputBuffer;

  Rectangle boundingBox;

  const unsigned labelWidth = MeasureText(label, fontSize);
  const unsigned labelY = (boundingBox.height - 24) / 2;

  InputCallback callback = [](const std::string &) {};

  unsigned cursorCounter = 0;
  bool isCursorVisible = true;
  unsigned cursorPosX = 0;

private:
  void drawCursor() {
    cursorCounter += GetFrameTime() * 1000.0;
    if (cursorCounter >= blinkingSpeed) {
      cursorCounter = 0;
      isCursorVisible = !isCursorVisible;
    }

    if (isCursorVisible) {
      int spacing = fontSize / 10;
      int startX = boundingBox.x + labelWidth;
      int textWidth =
          MeasureText(inputBuffer.substr(0, cursorPosX).c_str(), fontSize);

      int X = startX + textWidth + 1;
      DrawRectangle(X, labelY, cursorWidth, fontSize, BLACK);
    }
  }

  void handelInput() {
    for (int key = GetCharPressed(); key != 0; key = GetCharPressed()) {
      char buffer[2] = {(char)key, '\0'};
      inputBuffer.insert(cursorPosX, buffer);
      cursorPosX++;
      callback(inputBuffer);
    }
  }

  void handelShortCuts() {
    if (inputBuffer.length() == 0) {
      return;
    }

    int oldPos = cursorPosX;

    if (isCtrlHeld && IsKeyPressed(KEY_D)) {
      inputBuffer.clear();
    } else if (cursorPosX > 0 && IsKeyPressed(KEY_BACKSPACE) ||
               IsKeyPressedRepeat(KEY_BACKSPACE)) {
      inputBuffer.erase(cursorPosX - 1, 1);
      cursorPosX--;
    }

    if (isCtrlHeld && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT))) {

      if (IsKeyPressed(KEY_LEFT)) {
        if (cursorPosX != 0 && inputBuffer[cursorPosX - 1] == ' ')
          cursorPosX--;
        while (cursorPosX != 0 && inputBuffer[cursorPosX - 1] != ' ') {
          cursorPosX--;
        };
      }

      if (IsKeyPressed(KEY_RIGHT)) {
        if (inputBuffer[cursorPosX] == ' ')
          cursorPosX++;
        while (cursorPosX < inputBuffer.length() &&
               (cursorPosX == 0 || inputBuffer[cursorPosX] != ' ')) {
          cursorPosX++;
        }
      }

      cursorCounter = 0;
      isCursorVisible = true;
    }

    if (cursorPosX < inputBuffer.length() &&
        (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))) {
      cursorPosX++;
    } else if (cursorPosX > 0 &&
               (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))) {
      cursorPosX--;
    }

    if (oldPos != cursorPosX) {
      callback(inputBuffer);
    }
  }
};

struct Item {
  const char *title;
  void (*callback)();
};

void DrawItem(Item item, const Rectangle &BoundingBox) {
  DrawText(item.title, BoundingBox.x + 16, BoundingBox.y + (BoundingBox.height - (float)MeasureText(item.title, fontSize) / 2), fontSize, BLACK);
}

void drawItemList(Rectangle boundingBox, const std::vector<Item> &items,
                  unsigned itemHight) {
  for (size_t i = 0; i < items.size(); i++) {
    unsigned currentY =boundingBox.y + i * itemHight; 
    DrawItem(items[i], {boundingBox.x, static_cast<float>(currentY),
                        boundingBox.width, static_cast<float>(itemHight)});
    DrawLine(boundingBox.x, currentY + itemHight, boundingBox.x + boundingBox.width, currentY + itemHight, itemsSeperatorColor);
  }
}

int main() {
  InitWindow(0, 0, "zig is awesome");
  SetWindowState(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
  SetTargetFPS(30);

  const int monitor = GetCurrentMonitor();
  const int monitorWidth = GetMonitorWidth(monitor);
  const int monitorHeight = GetMonitorHeight(monitor);
  const int windowWidth = monitorWidth * 5 / 10;
  const int windowHeight = 600;

  //  TODO: Y can go out of bounds!!!
  SetWindowSize(windowWidth, windowHeight);
  SetWindowPosition((monitorWidth - windowWidth) / 2,
                    (monitorHeight - windowHeight) / 2 - 50);

  std::vector<Item> testItems;
  for (int i = 0; i < 10; i++) {
    testItems.push_back({.title = "test", .callback = []() {}});
  }

  auto trackKey = [](bool *state, int KEY) {
    if (IsKeyPressed(KEY))
      *state = true;
    if (IsKeyReleased(KEY))
      *state = false;
  };

  auto textInput = TextInput({16, 0, (float)windowWidth, 50}, inputLabel);
  const int textInputMargin = 8;

  while (!WindowShouldClose()) {
    trackKey(&isCtrlHeld, KEY_LEFT_CONTROL);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    textInput.render();

    DrawLine(0, textInput.getBoundingBox().height, windowWidth,
             textInput.getBoundingBox().height, seperatorColor);
  
    drawItemList({0, textInput.getBoundingBox().height + textInputMargin, (float)windowWidth, windowHeight - 50}, testItems, 50);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
