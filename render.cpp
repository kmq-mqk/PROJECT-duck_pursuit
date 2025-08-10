#include "render.hpp"
#include "map.hpp"

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


void Render(int cellSize, double rotateDuration) {
    BeginTextureMode(mazeTexture);
        ClearBackground(BLACK);

        DrawMaze(cellSize);
        DrawGoal(cellSize);
        DrawPlayer(cellSize);
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

void DrawMaze(int cellSize) {
    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            int posX = x * cellSize;
            int posY = y * cellSize;
            if (maze[x][y].topWall)  DrawLine(posX, posY, posX + cellSize, posY, WHITE);
            if (maze[x][y].rightWall)  DrawLine(posX + cellSize, posY, posX + cellSize, posY + cellSize, WHITE);
            if (maze[x][y].leftWall)  DrawLine(posX, posY , posX, posY + cellSize, WHITE);
            if (maze[x][y].bottomWall)  DrawLine(posX, posY + cellSize, posX + cellSize, posY + cellSize, WHITE);

        }
    }
}
void DrawPlayer(int cellSize) {
    DrawRectangle(player.x * cellSize + 2, player.y * cellSize + 2, cellSize - 4, cellSize - 4, GREEN);
}
void DrawGoal(int cellSize) {
    DrawRectangle(goal.x * cellSize + 2, goal.y * cellSize + 2, cellSize - 4, cellSize - 4, PINK);
}