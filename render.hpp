#pragma once

#include <raylib.h>

void DrawMaze(int cellSize);
void DrawPlayer(int cellSize);
void DrawGoal(int cellSize);
void Render(int cellSize, double rotateDuration);
void Rotate(double rotateDuration);
