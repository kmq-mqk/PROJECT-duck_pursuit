#pragma once

#include <raylib.h>

<<<<<<< Updated upstream
Vector2 MeasureAlterVec(double cellSize);
double MeasureCellSize();
void DrawMaze(Vector2 alterVec, double cellSize);
void DrawPlayer(Vector2 alterVec, double cellSize);
void DrawGoal(Vector2 alterVec, double cellSize);
void Render(Vector2 alterVec, double cellSize, double rotateDuration);
void Rotate(double rotateDuration);

#endif
=======
void DrawMaze(int cellSize);
void DrawPlayer(int cellSize);
void DrawGoal(int cellSize);
void Render(int cellSize, double rotateDuration);
void Rotate(double rotateDuration);
>>>>>>> Stashed changes
