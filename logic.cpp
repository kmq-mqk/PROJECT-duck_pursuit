#include "logic.hpp"
#include "map.hpp"

#include <stdlib.h>

// ALL NECESSARY GLOBAL VARIABLES FOR LOGIC COME FROM HERE
//bool gameWon = false;
//
//// EXTERNAL VARIABLES
//extern Position goal;
//extern MobiObj player;
//extern Cell** maze;
//extern int col, row;
//extern int screenWidth, screenHeight;
//extern double movingDuration;
//extern bool isRotating;
//extern Vector2 alterVec;
//extern double cellSize;
//extern double curAngle;
//extern double targetAngle;
//extern double dDeg;

//void MovePlayer(int dx, int dy) {
//    if (!player.isMoving) {
//        int x = (int)player.curPos.x;
//        int y = (int)player.curPos.y;
//        int newX = x + dx;
//        int newY = y + dy;
//        if (newX < 0 || newX >= col || newY < 0 || newY >= row) return;
//        if (dx == -1 && maze[x][y].leftWall) return;
//        if (dx == 1 && maze[x][y].rightWall) return;
//        if (dy == -1 && maze[x][y].topWall) return;
//        if (dy == 1 && maze[x][y].bottomWall) return;
//
//        player.isMoving = true;
//
//        player.tarPos.x = newX;
//        player.tarPos.y = newY;
//
//        player.speed.x = (player.tarPos.x - player.curPos.x) / movingDuration;
//        player.speed.y = (player.tarPos.y - player.curPos.y) / movingDuration;
//        player.dirX = 1 - 2 * (player.speed.x < 0);
//        player.dirY = 1 - 2 * (player.speed.y < 0);
//    }
//}
//
void InputMove(MobiObj* mobiObj, Maze* mazeInfo) {
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    int dir = -1;
    if (IsKeyPressed(KEY_UP))    dir = 0;
    if (IsKeyPressed(KEY_RIGHT)) dir = 1;
    if (IsKeyPressed(KEY_DOWN))  dir = 2;
    if (IsKeyPressed(KEY_LEFT))  dir = 3;

    if (dir != -1) {
		MoveArgs args = {mazeInfo, dx[dir], dy[dir]};
        mobiObj->base.Move(&(mobiObj->base), args);
    }
}

bool WinCheck(Vector2 objPos, Vector2 goalPos) {
	return ((int)objPos.x == (int)goalPos.x && (int)objPos.y == (int)goalPos.y);
}

//void ResetVal() {
//    if (maze != NULL) {
//		for (int i = 0; i < col; i++) {
//	        free(maze[i]);
//	        maze[i] = NULL;
//	    }
//		free(maze);
//		maze = NULL;
//	}
//
//	isRotating = false;
//	curAngle = targetAngle = dDeg = 0;
//	cellSize = 0;
//	alterVec = (Vector2){0, 0};
//}
