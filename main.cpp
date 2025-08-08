#include "map.hpp"
#include "render.hpp"
#include "logic.hpp"
#include "header.hpp"

// EXTERNAL VARIABLES
// map.cpp
extern Position player, goal;
extern Cell** maze;
extern int col, row;
// render.cpp
extern RenderTexture2D mazeTexture;
extern int screenWidth, screenHeight;
extern double curAngle;
extern double targetAngle;
extern double dDeg;
extern double lastAutoRotateTime;
extern float autoRotateInterval;
extern bool isRotating;
// logic.cpp
extern bool gameWon;

int main() {    
    InitWindow(screenWidth, screenHeight, "Penguin Test");
    SetTargetFPS(60);
    InitAudioDevice();
    GenerateMaze(5, 5);
    AddLoops(5);
    
    // 3 LINES BELOW ARE IMPORTANT  !!!
    lastAutoRotateTime = GetTime();
    autoRotateInterval = 5.0f;
    mazeTexture = LoadRenderTexture(screenWidth, screenHeight);

    while(!WindowShouldClose()) {
        InputMove();
        Render(50, 0.3);

        if (gameWon) break;
    }
    UnloadRenderTexture(mazeTexture);

    CloseWindow();

    return 0;
}