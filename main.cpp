#include "header.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


// EXTERNAL VARIABLES
extern RenderTexture2D mazeTexture;
extern Position player, goal;
extern Cell** maze;
extern int col, row;
extern int screenWidth, screenHeight;
extern float curAngle;
extern float targetAngle;
extern float dDeg;
extern double lastAutoRotateTime;
extern float autoRotateInterval;

float rotationAngle = 0.0f;
bool isRotating = false;
double rotationStartTime = 0.0;
float rotationDuration = 1.0f;
int rotationType = 0; // 0 = 90 CW, 1 = 90 CCW, 2 = 180
int directionOffset = 0; // Số lần xoay 90 độ CW (modulo 4)







// void RotateMaze90Clockwise() {
//     Cell temp[col][row];

//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             int nx = row - 1 - y;
//             int ny = x;

//             temp[nx][ny].visited = maze[x][y].visited;

//             // đổi hướng
//             temp[nx][ny].topWall = maze[x][y].leftWall;
//             temp[nx][ny].rightWall = maze[x][y].topWall;
//             temp[nx][ny].bottomWall = maze[x][y].rightWall;
//             temp[nx][ny].leftWall = maze[x][y].bottomWall;
//         }
//     }

//     // sao chép lại vào maze gốc
//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             maze[x][y] = temp[x][y];
//         }
//     }
// }

// void RotateMaze90CounterClockWise() {
//     Cell temp[col][row];

//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             int nx = y;
//             int ny = col - 1 - x;

//             temp[nx][ny].visited = maze[x][y].visited;

//             // Đổi hướng    
//             temp[nx][ny].topWall = maze[x][y].rightWall;
//             temp[nx][ny].rightWall = maze[x][y].bottomWall;
//             temp[nx][ny].bottomWall = maze[x][y].leftWall;
//             temp[nx][ny].leftWall = maze[x][y].topWall;
//         }
//     }

//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             maze[x][y] = temp[x][y];
//         }
//     }
// }

// void RotateMaze180() {
//     Cell temp[col][row];

//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             int nx = col - 1 - x;
//             int ny = row - 1 - y;
//             temp[nx][ny].visited = maze[x][y].visited;

//             temp[nx][ny].topWall = maze[x][y].bottomWall;
//             temp[nx][ny].bottomWall = maze[x][y].topWall;
//             temp[nx][ny].leftWall = maze[x][y].rightWall;
//             temp[nx][ny].rightWall = maze[x][y].leftWall;
//         }
//     }

//     for (int x = 0; x < col; x++) {
//         for (int y = 0; y < row; y++) {
//             maze[x][y] = temp[x][y];
//         }
//     }
// }


// void RotatePosition90Clockwise(Position &pos) {
//     int newX = row - 1 - pos.y;
//     int newY = pos.x;
//     pos.x = newX;
//     pos.y = newY;
// }

// void RotatePosition90CounterClockwise(Position &pos) {
//     int newX = pos.y;
//     int newY = col - 1 - pos.x;
//     pos.x = newX;
//     pos.y = newY;
// }
// void RotatePosition180(Position &pos) {
//     int newX = row - 1 - pos.x;
//     int newY = col - 1 - pos.y;
//     pos.x = newX;
//     pos.y = newY;
// }









int main() {
    int width = 800;
    int height = 600;
    InitWindow(width, height, "Penguin Test");
    mazeTexture = LoadRenderTexture(width, height);
    SetTargetFPS(60);
    InitAudioDevice();
    InitializeMaze();
    GenerateMaze(5, 5);
    AddLoops(5);
    player.x = 0, player.y = 0;
    goal.x = col - 1;
    goal.y = row - 1;
    
    // 2 LINES BELOW ARE IMPORTANT  !!!
    lastAutoRotateTime = GetTime();
    autoRotateInterval = 5.0f;

    while(!WindowShouldClose()) {
        if (!isRotating && GetTime() - lastAutoRotateTime >= autoRotateInterval) {
            isRotating = true;
            rotationStartTime = GetTime();
            lastAutoRotateTime = GetTime();
        
            rotationType = GetRandomValue(0, 2); // 0=CW, 1=CCW, 2=180
        
            switch (rotationType) {
                case 0: targetAngle = 90.0f; break;
                case 1: targetAngle = -90.0f; break;
                case 2: targetAngle = 180.0f; break;
            }
        }
        
        
        
        inputMove();
        if (isRotating) {
            float t = (GetTime() - rotationStartTime) / rotationDuration;
            if (t >= 1.0f) {
                isRotating = false;
                rotationAngle = 0.0f;
            
                // Xoay dữ liệu thật
                switch (rotationType) {
                    case 0: // 90 CW
                        directionOffset = (directionOffset + 1) % 4;
                        RotateMaze90Clockwise();
                        RotatePosition90Clockwise(player);
                        RotatePosition90Clockwise(goal);
                        break;
                    case 1: // 90 CCW
                        directionOffset = (directionOffset + 3) % 4;
                        RotateMaze90CounterClockWise();
                        RotatePosition90CounterClockwise(player);
                        RotatePosition90CounterClockwise(goal);
                        break;
                    case 2: // 180
                        directionOffset = (directionOffset + 2) % 4;
                        RotateMaze180();
                        RotatePosition180(player);
                        RotatePosition180(goal);
                        break;
                }
                
            }
            else {
                rotationAngle = t * targetAngle;
            }
            
        }
        
    //     if (gameWon) continue;
    // // Vẽ maze lên texture
    // BeginTextureMode(mazeTexture);
    //     ClearBackground(BLACK);
    //     DrawMaze();
    //     DrawPlayer();
    //     DrawGoal();
    // EndTextureMode();

    // // Sau đó xoay texture lên màn hình
    // BeginDrawing();
    // ClearBackground(BLACK);

    // Rectangle source = { 0, 0, (float)SCREEN_WIDTH, -(float)SCREEN_HEIGHT }; // cần - chiều cao
    // Rectangle dest = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    // Vector2 origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    // DrawTexturePro(mazeTexture.texture, source, dest, origin, rotationAngle, WHITE);

    // if (gameWon){
    //     int textWidth = MeasureText("You win!", 30);
    //     DrawText("You win!", SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2, 30, DARKGREEN);
    // }
        
    // EndDrawing();

    // }
    UnloadRenderTexture(mazeTexture);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}