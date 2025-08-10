#include "logic.hpp"
#include "map.hpp"

// ALL NECESSARY GLOBAL VARIABLES FOR LOGIC COME FROM HERE
bool gameWon = false;

// EXTERNAL VARIABLES
extern Position player, goal;
extern Cell** maze;
extern int col, row;
extern int screenWidth, screenHeight;

void MovePlayer(int dx, int dy) {
    int newX = player.x + dx;
    int newY = player.y + dy;
    if (newX < 0 || newX >= col || newY < 0 || newY >= row) return;
    if (dx == -1 && maze[player.x][player.y].leftWall) return;
    if (dx == 1 && maze[player.x][player.y].rightWall) return;
    if (dy == -1 && maze[player.x][player.y].topWall) return;
    if (dy == 1 && maze[player.x][player.y].bottomWall) return;

    player.x = newX;
    player.y = newY;

    if (player.x == goal.x && player.y == goal.y) gameWon = true;
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