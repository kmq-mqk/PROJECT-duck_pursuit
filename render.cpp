#include "render.hpp"
#include "object.h"
// #include "map.hpp"

#include <math.h>

// ALL NECESSARY GLOBAL VARIABLES FOR RENDERING COME FROM HERE
//int screenWidth = 800, screenHeight = 600;
//RenderTexture2D mazeTexture;
//double curAngle = 0;
//double targetAngle = 0;
//double dDeg = 0;
//double lastAutoRotateTime;
//float autoRotateInterval = 1.0f;
//bool isRotating = false;
//
//Vector2 alterVec;
//double cellSize;
//double movingDuration = 0.5;
//
//// EXTERNAL VARIABLES
//extern bool gameWon;
//
//extern int row, col;
//extern Cell** maze;
//extern Position goal;
//extern MobiObj player;


struct RenderList {
	size_t mobiCount;
	MobiObj** mobiList;
	size_t rotateCount;
	RotateObj** rotateList;
};


//void UpdateRender() {
//    screenWidth = GetScreenWidth();
//    screenHeight = GetScreenHeight();
//
//    cellSize = MeasureCellSize();
//    alterVec = MeasureAlterVec(cellSize);
//}
//
//void UpdateMobiObj(MobiObj& obj) {
//    if ((int)player.curPos.x == goal.x && (int)player.curPos.y == goal.y) gameWon = true;
//
//    if (obj.isMoving && obj.curPos.x * obj.dirX >= obj.tarPos.x * obj.dirX && obj.curPos.y * obj.dirY >= obj.tarPos.y * obj.dirY) {
//        obj.isMoving = false;
//        obj.speed = (Vector2){0, 0};
//        obj.curPos.x = obj.tarPos.x;
//        obj.curPos.y = obj.tarPos.y;
//    }
//    else if (obj.isMoving) {
//        obj.curPos.x += GetFrameTime() * obj.speed.x;
//        obj.curPos.y += GetFrameTime() * obj.speed.y;
//    }
//}

void UpdateTextureFromImage(Texture2D* texture, Image* img, int width, int height) {
	bool changed = (img->width != width || img->height != height);
	if (changed) {
		ImageResizeNN(img, width, height);
		UnloadTexture(*texture);
		*texture = LoadTextureFromImage(*img);
	}
}

void UpdateRender(RenderTexture* rt, MobiObj* mobi, RotateObj* rota, int screenWidth, int screenHeight) {
	double cell = MeasureCellSize((Vector2){(float)screenWidth, (float)screenHeight};
	bool changed = (rt->texture.width != width || rt->texture.height != height);
	if (changed) {
		UnloadRenderTexture(*rt);
		*rt = LoadRenderTexture(screenWidth, screenHeight);
		mobi->base.Resize(mobi, cell, cell);
		rota->base.Resize(rota, screenWidth, screenHeight);
	}
}

void Render(RenderList list, RenderTexture* lastTexture,  Vector2 alterVec, double cellSize) {
    BeginTextureMode(*lastTexture);
        ClearBackground(BLACK);

		for (size_t i = 0; i < list.mobiCount; i++) {
			list.mobiList[i]->base.Draw(&(list.mobiList[i]->base), alterVec);
		}
		for (size_t i = 0; i < list.rotateCount; i++) {
			list.rotateList[i]->base.Draw(&(list.rotateList[i]->base), alterVec);
		}
    EndTextureMode();

    // Rotate(rotateDuration);

    // Sau đó xoay texture lên màn hình
	float curAngle = (list.rotateList[0]->base.GetPos(&(list.rotateList[0]->base))).x;
	Vector2 size = list.rotateList[0]->base.GetSize(&(list.rotateList[0]->base));
	float width = size.x;
	float height = size.y;

    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle source = { 0, 0, width, -height }; // cần - chiều cao
        Rectangle dest = { width / 2, height / 2, width, height };
        Vector2 origin = { width / 2, height / 2 };

        DrawTexturePro((*lastTexture).texture, source, dest, origin, curAngle, WHITE);
    EndDrawing();
}
//
//int CycleAngle(int angle) {
//    if (angle < -360)
//        return angle + 360;
//    if (angle >= 360)
//        return angle - 360;
//
//    return angle;
//}
//void Rotate(double rotateDuration) {
//    int dir = 1 - 2 * (dDeg < 0);   // dir == 1 -> curAngle incrases; dir == -1 -> curAngle decreases
//
//    if (isRotating && curAngle * dir >= targetAngle * dir) {
//        curAngle = targetAngle = CycleAngle(targetAngle);
//        dDeg = 0;
//        isRotating = false;
//        
//        return;
//    }
//    if (!isRotating && GetTime() - lastAutoRotateTime >= autoRotateInterval) {
//            isRotating = true;
//            lastAutoRotateTime = GetTime();
//        
//            int rotationTypes[] = {-180, -90, 90, 180};
//
//            int size = sizeof(rotationTypes) / sizeof(rotationTypes[0]);
//        
//            int addAngle = rotationTypes[GetRandomValue(0, size - 1)];
//            dDeg = addAngle / rotateDuration;
//            targetAngle = curAngle + addAngle;
//            
//            isRotating = true;
//        }
//    if (isRotating && curAngle * dir < targetAngle * dir) {
//        curAngle += GetFrameTime() * dDeg;
//    }
//}

//void DrawMaze(Vector2 alterVec, double cellSize) {
//    for (int x = 0; x < col; x++) {
//        for (int y = 0; y < row; y++) {
//            int posX = x * cellSize + alterVec.x;
//            int posY = y * cellSize + alterVec.y;
//            if (maze[x][y].topWall)  DrawLine(posX, posY, posX + cellSize, posY, WHITE);
//            if (maze[x][y].rightWall)  DrawLine(posX + cellSize, posY, posX + cellSize, posY + cellSize, WHITE);
//            if (maze[x][y].leftWall)  DrawLine(posX, posY , posX, posY + cellSize, WHITE);
//            if (maze[x][y].bottomWall)  DrawLine(posX, posY + cellSize, posX + cellSize, posY + cellSize, WHITE);
//
//        }
//    }
//}
//void DrawPlayer(Vector2 alterVec, double cellSize) {
//    Vector2 pos = {alterVec.x + player.curPos.x * cellSize + 2, alterVec.y + player.curPos.y * cellSize + 2};
//    Vector2 size = {cellSize - 4, cellSize - 4};
//    DrawRectangleV(pos, size, GREEN);
//    // DrawRectangle(alterVec.x + player.x * cellSize + 2, alterVec.y + player.y * cellSize + 2, cellSize - 4, cellSize - 4, GREEN);
//}
void DrawGoal(Vector2 goalPos, Vector2 alterVec, double cellSize) {
    DrawRectangle(alterVec.x + goalPos.x * cellSize + 2, alterVec.y + goalPos.y * cellSize + 2, cellSize - 4, cellSize - 4, PINK);
}

double MeasureCellSize(Vector2 screenSize, Vector2 mazeSize) {
	int screenWidth = (int)screenSize.x, screenHeight = (int)screenSize.y;
	int col = (int)mazeSize.x, row = (int)mazeSize.y;

	// the code below is only for centering the maze on screen
	int minSize = (screenWidth < screenHeight) ? screenWidth : screenHeight;
	return minSize / sqrt(col * col + row * row);
}

Vector2 MeasureAlterVec(Vector2 screenSize, Vector2 mazeSize, double cellSize) {
	float screenWidth = screenSize.x, screenHeight = screenSize.y;
	int col = (int)mazeSize.x, row = (int)mazeSize.y;

	// the code below is only for centering the maze on screen
	float halfMazeWidth = cellSize * col / 2.0f;
	float halfMazeHeight = cellSize * row / 2.0f;

	return (Vector2) {screenWidth / 2.0f - halfMazeWidth, screenHeight / 2.0f - halfMazeHeight};
}
