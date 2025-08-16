#include "logic.hpp"
#include "map.hpp"

#include <raylib.h>
#include <stdio.h>

// ALL NECESSARY GLOBAL VARIABLES FOR LOGIC COME FROM HERE
bool gameWon = false;

// EXTERNAL VARIABLES
extern Position goal;
extern MobiObj player;
extern Cell** maze;
extern int col, row;
extern int screenWidth, screenHeight;
extern double movingDuration;


void MovePlayer(int dx, int dy) {
    if (!player.isMoving) {
        int x = (int)player.curPos.x;
        int y = (int)player.curPos.y;
        int newX = x + dx;
        int newY = y + dy;
        if (newX < 0 || newX >= col || newY < 0 || newY >= row) return;
        if (dx == -1 && maze[x][y].leftWall) return;
        if (dx == 1 && maze[x][y].rightWall) return;
        if (dy == -1 && maze[x][y].topWall) return;
        if (dy == 1 && maze[x][y].bottomWall) return;

        player.isMoving = true;

        player.tarPos.x = newX;
        player.tarPos.y = newY;

        player.speed.x = (player.tarPos.x - player.curPos.x) / movingDuration;
        player.speed.y = (player.tarPos.y - player.curPos.y) / movingDuration;
        player.dirX = 1 - 2 * (player.speed.x < 0);
        player.dirY = 1 - 2 * (player.speed.y < 0);

        PlayerInfo();
    }
}

void InputMove() {
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    int dir = -1;
    if (IsKeyPressed(KEY_UP))    dir = 0;
    if (IsKeyPressed(KEY_RIGHT)) dir = 1;
    if (IsKeyPressed(KEY_DOWN))  dir = 2;
    if (IsKeyPressed(KEY_LEFT))  dir = 3;

    if (dir != -1) {
        MovePlayer(dx[dir], dy[dir]);
    }
}
