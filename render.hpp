#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include "map.hpp"

#include <raylib.h>

void UpdateRender();
void UpdateMobiObj(MobiObj&);

Vector2 MeasureAlterVec(double cellSize);
double MeasureCellSize();
void DrawMaze(Vector2 alterVec, double cellSize);
void DrawPlayer(Vector2 alterVec, double cellSize);
void DrawGoal(Vector2 alterVec, double cellSize);
void Render(Vector2 alterVec, double cellSize, double rotateDuration);
void Rotate(double rotateDuration);

#endif
