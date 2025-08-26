#define RAYGUI_IMPLEMENTATION
#include "header.hpp"

/* ------------------
    EXTERNAL VARIABLES

// map.cpp
extern Position goal;
extern MobiObj player;
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
extern Vector2 alterVec;
extern double cellSize;
// logic.cpp
extern bool gameWon;

------------------ */

Vector2 initScreenSize = {800.0f, 600.0f};

int main() {    
    InitWindow(initScreenSize.x, initScreenSize.y, "Penguin Test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    GameStart(); // chạy toàn bộ vòng đời game: OPENING → GAMEPLAY → ENDING

    CloseWindow();
    return 0;
}
