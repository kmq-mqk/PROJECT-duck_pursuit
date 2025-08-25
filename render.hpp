#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include "map.hpp"

#include <raylib.h>

typedef struct RenderList RenderList;

void UpdateTextureFromImage(Texture2D*, Image*, int screenWidth, int screenHeight);
void UpdateRender(RenderTexture2D*, MobiObj*, RotateObj*);
//void UpdateMobiObj(MobiObj&);

Vector2 MeasureAlterVec(Vector2 screenSize, Vector2 mazeSize, double cellSize);
double MeasureCellSize(Vector2 screenSize, Vector2 mazeSize);
//void DrawMaze(Vector2 alterVec, double cellSize);
//void DrawPlayer(Vector2 alterVec, double cellSize);
void DrawGoal(Vector2 goalPos, Vector2 alterVec, double cellSize);
void Render(RenderList list, RenderTexture* lastTexture,  Vector2 alterVec, double cellSize);
//void Rotate(double rotateDuration);

#endif
