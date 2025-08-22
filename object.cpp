#include "object.h"
#include <stdio.h>
#include <stdlib.h>

struct Cell {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
};

struct Maze {
	int col, row;
	Cell** maze;	// _maze[col][row]
};
struct MoveArgs {
	Maze maze;
	int dx, dy;
};

// ******* PLAYER'S SECTION BEGINS *******

// args: Obj* mobiObj, Maze mazeInfo, int dx, int dy
static void MobiObj_Move(Obj* obj, const MoveArgs args) {
	Maze mazeInfo = args.maze;
	int dx = args.dx;
	int dy = args.dy;
	MobiObj* mobiObj = (MobiObj*)obj;

	if (mobiObj->_base._isMoving || (dx == dy && 1 == dx))
		return;

	const int x = (int)mobiObj->_curPos.x;
	const int y = (int)mobiObj->_curPos.y;
	const int newX = x + dx;
	const int newY = y + dy;
	const int col = mazeInfo.col;
	const int row = mazeInfo.row;
	Cell** maze = mazeInfo.maze;
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

static void MobiObj_Draw(Obj* obj, Vector2 renderPos) {
	MobiObj* mobiObj = (MobiObj*)obj;
	Vector2 pos = {obj->_rt.texture.width / 2.0f + mobiObj->_curPos.x, obj->_rt.texture.height / 2.0f + mobiObj->_curPos.y}; 
	float radius = (obj->_rt.texture.width < obj->_rt.texture.height) ? obj->_rt.texture.width / 2.0f : obj->_rt.texture.height / 2.0f;

	// when we have the sprite for our dear obj, we don't need the block below
	BeginTextureMode(obj->_rt);
		DrawCircleV(pos, radius, GREEN);
	EndTextureMode();

	DrawTextureEx(obj->_rt.texture, renderPos, 0.0f, 1.0f, WHITE);
}

static void MobiObj_Update(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;

	if (obj->_isMoving && mobiObj->_curPos.x * mobiObj->_dirX >= mobiObj->_tarPos.x * mobiObj->_dirX && mobiObj->_curPos.y * mobiObj->_dirY >= mobiObj->_tarPos.y * mobiObj->_dirY) {
		obj->_isMoving = false;
		mobiObj->_speed = (Vector2){0, 0};
		mobiObj->_curPos.x = mobiObj->_tarPos.x;
		mobiObj->_curPos.y = mobiObj->_tarPos.y;
	}
	else if (obj->_isMoving) {
		mobiObj->_curPos.x += GetFrameTime() * mobiObj->_speed.x;
		mobiObj->_curPos.y += GetFrameTime() * mobiObj->_speed.y;
	}
}

static void MobiObj_Resize(Obj* obj, int width, int height) {
	int newWidth = (width != -1) ? width : obj->_rt.texture.width;
	int newHeight = (height != -1) ? height : obj->_rt.texture.height;
	bool changed = (newWidth != obj->_rt.texture.width || newHeight != obj->_rt.texture.height);

	MobiObj* mobiObj = (MobiObj*)obj;
	if (changed) {
		if (IsImageValid(mobiObj->_sprite))
			ImageResize(&mobiObj->_sprite, newWidth, newHeight);
		else {
			obj->_rt.texture.width = newWidth;
			obj->_rt.texture.height = newHeight;
		}
	}
}

static void MobiObj_Free(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;

	if (IsImageValid(mobiObj->_sprite))
		UnloadImage(mobiObj->_sprite);
	if (IsRenderTextureValid(obj->_rt))
		UnloadRenderTexture(obj->_rt);

	free(mobiObj);
}

// remember to add image loading
Obj* New_MobiObj(double movingDuration, int width, int height) {
	MobiObj* mobiObj = (MobiObj*)malloc(sizeof(MobiObj));
	if (!mobiObj)
		return NULL;

	mobiObj->_base = (Obj){
		0,
		movingDuration,
		LoadRenderTexture(width, height),

		MobiObj_Move,
		MobiObj_Draw,
		MobiObj_Update,
		MobiObj_Resize,
		MobiObj_Free,
	};
	
	mobiObj->_sprite = (Image){0};
	mobiObj->_dirX = mobiObj->_dirY = 0;
	mobiObj->_speed = mobiObj->_curPos = mobiObj->_tarPos = (Vector2){0, 0};

	return (Obj*)mobiObj;
}

// ******* PLAYER'S SECTION ENDS *******


// ******* MAZE'S SECTION BEGINS *******

static void RotateObj_Move(Obj* obj, MoveArgs args) {
}

static void RotateObj_Draw(Obj* obj, Vector2 renderPos) {
}

static void RotateObj_Update(Obj* obj) {
}

static void RotateObj_Resize(Obj* obj, int width, int height) {
}

static void RotateObj_Free(Obj* obj) {
	RotateObj* rotateObj = (RotateObj*)obj;

	if (IsRenderTextureValid(obj->_rt))
		UnloadRenderTexture(obj->_rt);
	Cell** maze = rotateObj->_mazeInfo->maze;
	int col = rotateObj->_mazeInfo->col;
	for (int i = 0; i < col; i++) {
		free(maze[i]);
		maze[i] = NULL;
	}
	free(maze);
	maze = NULL;

	free(rotateObj);
}

// remember to add file loading here (maze loading)
Obj* New_RotateObj(double movingDuration, int width, int height) {
	RotateObj* obj = (RotateObj*)malloc(sizeof(RotateObj));
	if (!obj)
		return NULL;

	obj->_base = (Obj){
		0,
		movingDuration,
		LoadRenderTexture(width, height),

		RotateObj_Move,
		RotateObj_Draw,
		RotateObj_Update,
		RotateObj_Resize,
		RotateObj_Free,
	};

	obj->_dir = 0;
	obj->_speed = obj->_curAngle = obj->_tarAngle = 0.0;

	return (Obj*)obj;
}

// ******* MAZE'S SECTION ENDS *******
