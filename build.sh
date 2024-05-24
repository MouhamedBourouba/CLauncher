#!/bin/bash

SOURCE_FILE="clauncher.cpp"
OUTPUT_EXECUTABLE="clan"
RAYLIB_PATH="raylib/"

# Compile command
g++ -std=c++17 -o "$OUTPUT_EXECUTABLE" "$SOURCE_FILE" -L"$RAYLIB_PATH" -lraylib && ./clan

