#include "logic.hpp"
#include "map.hpp"

#include <stdlib.h>


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

void Rotate(RenderList list) {
	MobiObj** mobiLs = list.mobiList;
	RotaObj** rotaLs = list.rotaList;
	size_t mobiCnt=  list.mobiCount;
	size_t rotaCnt=  list.rotaCount;


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
