#include "render.hpp"


// ALL NECESSARY GLOBAL VARIABLES FOR RENDERING COME FROM HERE
int screenWidth = 600, screenHeight = 600;
RenderTexture2D mazeTexture;
float curAngle = 0.0f;
float targetAngle = 0.0f;
float dDeg = 0.0f;
double lastAutoRotateTime;
float autoRotateInterval = 5.0f;

void Render(int cellSize, float rotateDuration) {
    Rotate(rotateDuration);

    BeginTextureMode(mazeTexture);
        ClearBackground(BLACK);

        DrawMaze(cellSize);
        DrawGoal(cellSize);
        DrawPlayer(cellSize);
    EndTextureMode();

    // Sau đó xoay texture lên màn hình
    BeginDrawing();
        ClearBackground(BLACK);

        Rectangle source = { 0, 0, (float)screenWidth, -(float)screenHeight }; // cần - chiều cao
        Rectangle dest = { screenWidth / 2, screenHeight / 2, (float)screenWidth, (float)screenHeight };
        Vector2 origin = { screenWidth / 2, screenHeight / 2 };

        DrawTexturePro(mazeTexture.texture, source, dest, origin, rotationAngle, WHITE);
    EndDrawing();
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