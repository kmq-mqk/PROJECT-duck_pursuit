#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void DrawMaze(int cellSize);
void DrawPlayer(int cellSize);
void DrawGoal(int cellSize);
void Render(int cellSize, float rotateDuration);    // STILL UNDER CONSTRUCTION
void Rotate(float rotateDuration);      // STILL UNDER CONSTRUCTION

#endif