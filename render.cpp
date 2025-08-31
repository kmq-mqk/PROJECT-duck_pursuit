#include "render.hpp"
#include "object.h"

#include <math.h>


void UpdateTextureFromImage(Texture2D* texture, Image* img, int width, int height) {
	bool changed = (img->width != width || img->height != height);
	if (changed) {
		ImageResizeNN(img, width, height);
		UnloadTexture(*texture);
		*texture = LoadTextureFromImage(*img);
	}
}

void UpdateRender(RenderTexture* rt, RenderList list, int screenWidth, int screenHeight) {
//	Vector2 screenSize = {(float)screenWidth, (float)screenHeight};
//	Maze* mazeInfo = rota->GetMazeInfo(rota);
//	Vector2 mazeSize = {(float)mazeInfo->col, (float)mazeInfo->row};
//	double cell = MeasureCellSize(screenSize, mazeSize);
//	
//	float newW = cell 
//	bool changed = (rt->texture.width != screenWidth || rt->texture.height != screenHeight);
//	if (changed) {
//		UnloadRenderTexture(*rt);
//		*rt = LoadRenderTexture(screenWidth, screenHeight);
//		rota->base.Resize((Obj*)rota, screenWidth, screenHeight);
//		Maze* mazeInfo = ((RotaObj*)rota)->GetMazeInfo(rota);
//		double cell = MeasureCellSize((Vector2){screenWidth, screenHeight}, (Vector2){mazeInfo->col, mazeInfo->row});
//		mobi->base.Resize((Obj*)mobi, cell, cell);
//	}

	MobiObj** mobiLs = list.mobiList;
	RotaObj** rotaLs = list.rotaList;
	size_t mobiCnt = list.mobiCount;
	size_t rotaCnt = list.rotaCount;

	for (size_t cnt = 0; cnt < rotaCnt; cnt++) {
		((Obj*)rotaLs[cnt])->Resize((Obj*)rotaLs[cnt], screenWidth, screenHeight);
	}
	Vector2 cellSize = ((Obj*)rotaLs[0])->GetSize((Obj*)rotaLs[0]);
	for (size_t cnt = 0; cnt < mobiCnt; cnt++) {
		((Obj*)mobiLs[cnt])->Resize((Obj*)mobiLs[cnt], (int)cellSize.x, (int)cellSize.y);
	}
	Maze* mazeInfo = rotaLs[0]->GetMazeInfo(rotaLs[0]);
	float rtW = cellSize.x * mazeInfo->col + 5;
	float rtH = cellSize.y * mazeInfo->row + 5;
	if (rtW != rt->texture.width || rtH != rt->texture.height) {
		UnloadRenderTexture(*rt);
		*rt = LoadRenderTexture(rtW, rtH);
	}
}

void Render(RenderList list, RenderTexture* lastTexture,  Vector2 alterV) {
    BeginTextureMode(*lastTexture);
        ClearBackground(BLACK);

		for (size_t i = 0; i < list.rotaCount; i++) {
			list.rotaList[i]->base.Draw(&(list.rotaList[i]->base));
		}

		for (size_t i = 0; i < list.mobiCount; i++) {
			list.mobiList[i]->base.Draw(&(list.mobiList[i]->base));
		}
    EndTextureMode();

    // Sau đó xoay texture lên màn hình
	float curAngle = (list.rotaList[0]->base.GetPos(&(list.rotaList[0]->base))).x;
	
	int width = lastTexture->texture.width;
	int height = lastTexture->texture.height;

    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle source = { 0, 0, width, -height }; // cần - chiều cao
        Rectangle dest = { alterV.x + width / 2, alterV.y + height / 2, width, height };
        Vector2 origin = { width / 2, height / 2 };

        DrawTexturePro(lastTexture->texture, source, dest, origin, curAngle, WHITE);
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
