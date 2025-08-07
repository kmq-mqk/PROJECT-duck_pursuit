#include "map.hpp"
#include "render.hpp"
#include "logic.hpp"

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


int main() {    
    // 3 LINES BELOW ARE IMPORTANT  !!!
    lastAutoRotateTime = GetTime();
    autoRotateInterval = 5.0f;
    mazeTexture = LoadRenderTexture(screenWidth, screenHeight);

    while(!WindowShouldClose()) {
        GameStart();
    }
    UnloadRenderTexture(mazeTexture);

    CloseWindow();
    return 0;
}