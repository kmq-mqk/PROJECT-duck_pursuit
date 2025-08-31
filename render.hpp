#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include "map.hpp"
#include "object.h"

#include <raylib.h>
#include <stdio.h>



void UpdateTextureFromImage(Texture2D*, Image*, int screenWidth, int screenHeight);
void UpdateRender(RenderTexture* rt, RenderList list, int screenWidth, int screenHeight);

Vector2 MeasureAlterVec(Vector2 screenSize, Vector2 mazeSize, double cellSize);
double MeasureCellSize(Vector2 screenSize, Vector2 mazeSize);
void Render(RenderList list, RenderTexture* lastTexture,  Vector2 alterVec);

#endif
