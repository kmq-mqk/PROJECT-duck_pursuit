#include "object.h"
#include <stdio.h>
#include <stdlib.h>

// ******* PLAYER'S SECTION BEGINS *******

// args: Obj* mobiObj, Maze mazeInfo, int dx, int dy
static void MobiObj_Move(Obj* obj, ...) {
	va_list args;
	va_start(args, obj);

	Maze mazeInfo = va_arg(args, Maze);
	int dx = va_arg(args, int);
	int dy = va_arg(args, int);
	MobiObj* mobiObj = (MobiObj*)obj;

	if (mobiObj->_base._isMoving || (dx == dy && 1 == dx))
		return;

	const int x = (int)mobiObj->_curPos.x;
	const int y = (int)mobiObj->_curPos.y;
	const int newX = x + dx;
	const int newY = y + dy;
	const int col = mazeInfo._col;
	const int row = mazeInfo._row;
	Cell** maze = mazeInfo._maze;
	const double movingDuration = mobiObj->_base._movingDuration;

	if (newX < 0 || newX >= col || newY < 0 || newY >= row) return;
	if (dx == -1 && maze[x][y].leftWall) return;
	if (dx == 1 && maze[x][y].rightWall) return;
	if (dy == -1 && maze[x][y].topWall) return;
	if (dy == 1 && maze[x][y].bottomWall) return;
	
	mobiObj->_base._isMoving = true;
	
	mobiObj->_tarPos.x = newX;
	mobiObj->_tarPos.y = newY;
	
	mobiObj->_speed.x = (mobiObj->_tarPos.x - mobiObj->_curPos.x) / movingDuration;
	mobiObj->_speed.y = (mobiObj->_tarPos.y - mobiObj->_curPos.y) / movingDuration;
	mobiObj->_dirX = 1 - 2 * (mobiObj->_speed.x < 0);
	mobiObj->_dirY = 1 - 2 * (mobiObj->_speed.y < 0);
}

static void MobiObj_Draw(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;
}

static void MobiObj_Update(Obj* obj

static void MobiObj_Free(Obj* mobiObj) {
}

Obj* New_MobiObj(double movingDuration, int width, int height) {
	MobiObj* obj = (MobiObj*)malloc(sizeof(MobiObj));
	if (!obj)
		return NULL;

	obj->_base = (Obj){
		._isMoving = 0,
		._movingDuration = movingDuration;
		._width = width;
		._height = height;

		.Move = MobiObj_Move,
//		.Draw = MobiObj_Draw,
//		.Update = MobiObj_Update,
//		.Free = MobiObj_Free,
	};
	
	obj->_dirX = obj->_dirY = 0;
	obj->_speed = obj->_curPos = obj->_tarPos = (Vector2){0, 0};

	return (Obj*)obj;
}

// ******* PLAYER'S SECTION ENDS *******


// ******* MAZE'S SECTION BEGINS *******
//
//static void RotateObj_Move(Obj* rotateObj) {
//}
//
//static void RotateObj_Draw(Obj* rotateObj) {
//}
//
//static void RotateObj_Free(Obj* rotateObj) {
//}
//
//Obj* New_RotateObj(int width, int height) {
//	RotateObj* obj = (RotateObj*)malloc(sizeof(RotateObj));
//	if (!obj)
//		return NULL;
//
//	obj->_base = (Obj){
//		._isMoving = 0,
//		._movingDuration = movingDuration;
//		._width = width;
//		._height = height;

//		
//		.Move = RotateObj_Move,
//		.Draw = RotateObj_Draw,
//		.Update = RotateObj_Update,
//		.Free = RotateObj_Free,
//	};
//
//	obj->_dir = 0;
//	obj->_speed = obj->_curAngle = obj->_tarAngle = 0.0;
//	obj->_texture = LoadRenderTexture(width, height);
//
//	return (Obj*)obj;
//}
//
//// ******* MAZE'S SECTION ENDS *******
