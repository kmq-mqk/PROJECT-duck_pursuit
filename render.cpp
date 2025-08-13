#include "render.hpp"
#include "map.hpp"

#include <math.h>

// ALL NECESSARY GLOBAL VARIABLES FOR RENDERING COME FROM HERE
int screenWidth = 600, screenHeight = 600;
RenderTexture2D mazeTexture;
double curAngle = 0;
double targetAngle = 0;
double dDeg = 0;
double lastAutoRotateTime;
float autoRotateInterval = 1.0f;
bool isRotating = false;

// EXTERNAL VARIABLES
extern int row, col;
extern Cell** maze;
extern Position player, goal;


void UpdateRender(Vector2& alterVec, double cellSize) {
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	cellSize = MeasureCellSize();
	alterVec = MeasureAlterVec(cellSize);
}

void Render(Vector2 alterVec, double cellSize, double rotateDuration) {
    BeginTextureMode(mazeTexture);
        ClearBackground(BLACK);

        DrawMaze(alterVec, cellSize);
        DrawGoal(alterVec, cellSize);
        DrawPlayer(alterVec, cellSize);
    EndTextureMode();

    Rotate(rotateDuration);

    // Sau đó xoay texture lên màn hình
    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle source = { 0, 0, (float)screenWidth, -(float)screenHeight }; // cần - chiều cao
        Rectangle dest = { (float)screenWidth / 2, (float)screenHeight / 2, (float)screenWidth, (float)screenHeight };
        Vector2 origin = { (float)screenWidth / 2, (float)screenHeight / 2 };

        DrawTexturePro(mazeTexture.texture, source, dest, origin, curAngle, WHITE);
    EndDrawing();
}

int CycleAngle(int angle) {
    if (angle < -360)
        return angle + 360;
    if (angle >= 360)
        return angle - 360;

    return angle;
}
void Rotate(double rotateDuration) {
    int dir = 1 - 2 * (dDeg < 0);   // dir == 1 -> curAngle incrases; dir == -1 -> curAngle decreases

    if (isRotating && curAngle * dir >= targetAngle * dir) {
        curAngle = targetAngle = CycleAngle(targetAngle);
        dDeg = 0;
        isRotating = false;
        
        return;
    }
    if (!isRotating && GetTime() - lastAutoRotateTime >= autoRotateInterval) {
            isRotating = true;
            lastAutoRotateTime = GetTime();
        
            int rotationTypes[] = {-180, -90, 90, 180};

            int size = sizeof(rotationTypes) / sizeof(rotationTypes[0]);
        
            int addAngle = rotationTypes[GetRandomValue(0, size - 1)];
            dDeg = addAngle / rotateDuration;
            targetAngle = curAngle + addAngle;
            
            isRotating = true;
        }
    if (isRotating && curAngle * dir < targetAngle * dir) {
        curAngle += GetFrameTime() * dDeg;
    }
}

void DrawMaze(Vector2 alterVec, double cellSize) {
    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            int posX = x * cellSize + alterVec.x;
            int posY = y * cellSize + alterVec.y;
            if (maze[x][y].topWall)  DrawLine(posX, posY, posX + cellSize, posY, WHITE);
            if (maze[x][y].rightWall)  DrawLine(posX + cellSize, posY, posX + cellSize, posY + cellSize, WHITE);
            if (maze[x][y].leftWall)  DrawLine(posX, posY , posX, posY + cellSize, WHITE);
            if (maze[x][y].bottomWall)  DrawLine(posX, posY + cellSize, posX + cellSize, posY + cellSize, WHITE);

        }
    }
}
void DrawPlayer(Vector2 alterVec, double cellSize) {
    DrawRectangle(alterVec.x + player.x * cellSize + 2, alterVec.y + player.y * cellSize + 2, cellSize - 4, cellSize - 4, GREEN);
}
void DrawGoal(Vector2 alterVec, double cellSize) {
    DrawRectangle(alterVec.x + goal.x * cellSize + 2, alterVec.y + goal.y * cellSize + 2, cellSize - 4, cellSize - 4, PINK);
}

double MeasureCellSize() {
	// the code below is only for centering the maze on screen
	int minSize = (screenWidth < screenHeight) ? screenWidth : screenHeight;
	return minSize / sqrt(col * col + row * row);
}

Vector2 MeasureAlterVec(double cellSize) {
	// the code below is only for centering the maze on screen
	double halfMazeWidth = cellSize * col / 2.0;
	double halfMazeHeight = cellSize * row / 2.0;

	return (Vector2) {screenWidth / 2.0 - halfMazeWidth, screenHeight / 2.0 - halfMazeHeight};
}
