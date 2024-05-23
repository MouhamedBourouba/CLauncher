#!/bin/bash

SOURCE_FILE="main.cpp"
OUTPUT_EXECUTABLE="clan"
RAYLIB_PATH="raylib/"

# Compile command
g++ -std=c++17 -o "$OUTPUT_EXECUTABLE" "$SOURCE_FILE" -L"$RAYLIB_PATH" -lraylib && ./clan

