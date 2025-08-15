#define RAYGUI_IMPLEMENTATION
#include "header.hpp"

/* ------------------
    EXTERNAL VARIABLES

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

------------------ */

int main() {    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penguin Test");
    SetTargetFPS(60);

    GameStart(); // chạy toàn bộ vòng đời game: OPENING → GAMEPLAY → ENDING

    CloseWindow();
    return 0;
}