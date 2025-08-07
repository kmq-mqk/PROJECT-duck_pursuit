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


int main() {    
    

    GameStart();

    CloseWindow();
    return 0;
}