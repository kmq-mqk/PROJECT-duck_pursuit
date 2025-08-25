#include "object.h"
#include "map.hpp"
#include <stdio.h>
#include <stdlib.h>


static Vector2 Obj_GetSize(Obj* obj) {
	return (Vector2){(float)obj->_rt.texture.width, (float)obj->_rt.texture.height};
}

// ******* PLAYER'S SECTION BEGINS *******

static Vector2 MobiObj_GetPos(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;
	return mobiObj->_curPos;
}

// args: Obj* mobiObj, Maze mazeInfo, int dx, int dy
static void MobiObj_Move(Obj* obj, const MoveArgs args) {
	Maze* mazeInfo = args.mazeInfo;
	int dx = args.dx;
	int dy = args.dy;
	MobiObj* mobiObj = (MobiObj*)obj;

	if (mobiObj->base._isMoving || (dx == dy && 1 == dx))
		return;

	const int x = (int)mobiObj->_curPos.x;
	const int y = (int)mobiObj->_curPos.y;
	const int newX = x + dx;
	const int newY = y + dy;
	const int col = mazeInfo->col;
	const int row = mazeInfo->row;
	Cell** maze = mazeInfo->maze;
	const double movingDuration = mobiObj->base._movingDuration;

	if (newX < 0 || newX >= col || newY < 0 || newY >= row) return;
	if (dx == -1 && maze[x][y].leftWall) return;
	if (dx == 1 && maze[x][y].rightWall) return;
	if (dy == -1 && maze[x][y].topWall) return;
	if (dy == 1 && maze[x][y].bottomWall) return;
	
	mobiObj->base._isMoving = true;
	
	mobiObj->_tarPos.x = newX;
	mobiObj->_tarPos.y = newY;
	
	mobiObj->_speed.x = (mobiObj->_tarPos.x - mobiObj->_curPos.x) / movingDuration;
	mobiObj->_speed.y = (mobiObj->_tarPos.y - mobiObj->_curPos.y) / movingDuration;
	mobiObj->_dirX = 1 - 2 * (mobiObj->_speed.x < 0);
	mobiObj->_dirY = 1 - 2 * (mobiObj->_speed.y < 0);
}

static void MobiObj_Draw(Obj* obj, Vector2 alterV) {
	MobiObj* mobiObj = (MobiObj*)obj;
	Vector2 pos = {obj->_rt.texture.width / 2.0f, obj->_rt.texture.height / 2.0f}; 
	float radius = (obj->_rt.texture.width < obj->_rt.texture.height) ? obj->_rt.texture.width / 2.0f : obj->_rt.texture.height / 2.0f;

	// when we have the sprite for our dear obj, we don't need the block below
	BeginTextureMode(obj->_rt);
		DrawCircleV(pos, radius, GREEN);
	EndTextureMode();

	Vector2 renderPos = {alterV.x + mobiObj->_curPos.x * obj->_rt.texture.width, alterV.y + mobiObj->_curPos.y * obj->_rt.texture.height};
	DrawTextureEx(obj->_rt.texture, renderPos, 0.0f, 1.0f, WHITE);
}

static void MobiObj_Update(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;

	if (obj->_isMoving && mobiObj->_curPos.x * mobiObj->_dirX >= mobiObj->_tarPos.x * mobiObj->_dirX && mobiObj->_curPos.y * mobiObj->_dirY >= mobiObj->_tarPos.y * mobiObj->_dirY) {
		obj->_isMoving = 0;
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
		if (IsImageValid(mobiObj->_sprite)) {
			ImageResizeNN(&mobiObj->_sprite, newWidth, newHeight);
			if (IsRenderTextureValid(obj->_rt))
				UnloadRenderTexture(obj->_rt);

			obj->_rt = LoadRenderTexture(newWidth, newHeight);

			Texture tmp = LoadTextureFromImage(mobiObj->_sprite);
			BeginTextureMode(obj->_rt);
				ClearBackground(BLANK);
				DrawTexture(tmp, 0, 0, WHITE);
			EndTextureMode();
			UnloadTexture(tmp);
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
Obj* New_MobiObj(const char* spriteFile, double movingDuration, int width, int height) {
	MobiObj* mobiObj = (MobiObj*)malloc(sizeof(MobiObj));
	if (!mobiObj)
		return NULL;

	// BUILD RENDERTEXTURE FOR MOBIOBJ UP HERE

	mobiObj->base = (Obj){
		0,
		movingDuration,
		LoadRenderTexture(width, height),

		MobiObj_GetPos,
		Obj_GetSize,
		MobiObj_Move,
		MobiObj_Draw,
		MobiObj_Update,
		MobiObj_Resize,
		MobiObj_Free,
	};
	
	mobiObj->_sprite = LoadImage(spriteFile);
	ImageResizeNN(&mobiObj->_sprite, width, height);
	BeginTextureMode(mobiObj->base._rt);
		Texture tmp = LoadTextureFromImage(mobiObj->_sprite);
		DrawTextureEx(tmp, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
	EndTextureMode();

	mobiObj->base._rt.texture = LoadTextureFromImage(mobiObj->_sprite);
	mobiObj->_dirX = mobiObj->_dirY = 0;
	mobiObj->_speed = mobiObj->_curPos = mobiObj->_tarPos = (Vector2){0, 0};

	return (Obj*)mobiObj;
}

// ******* PLAYER'S SECTION ENDS *******


// ******* MAZE'S SECTION BEGINS *******

static void DrawMaze(RotateObj rotateObj) {
	int col = rotateObj._mazeInfo->col;
	int row = rotateObj._mazeInfo->row;
	Cell** maze = rotateObj._mazeInfo->maze;
	float cellWidth = (float)rotateObj.base._rt.texture.width / col;
	float cellHeight = (float)rotateObj.base._rt.texture.height / row;

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            float posX = cellWidth * x;
            float posY = cellHeight * y;

            if (maze[x][y].topWall) {
				Vector2 start = {posX, posY};
				Vector2 end = {posX + cellWidth, posY};
			   	DrawLineV(start, end, WHITE);
			}
            if (maze[x][y].rightWall) {
				Vector2 start = {posX + cellWidth, posY};
				Vector2 end = {posX + cellWidth, posY + cellHeight};
			   	DrawLineV(start, end, WHITE);
			}
            if (maze[x][y].leftWall) {
				Vector2 start = {posX, posY};
				Vector2 end = {posX, posY + cellHeight};
			   	DrawLineV(start, end, WHITE);
			}
            if (maze[x][y].bottomWall) {
				Vector2 start = {posX, posY + cellHeight};
				Vector2 end = {posX + cellWidth, posY + cellHeight};
			   	DrawLineV(start, end, WHITE);
			}
        }
    }
}

static Vector2 RotateObj_GetPos(Obj* obj) {
	RotateObj* rotateObj = (RotateObj*)obj;
	return (Vector2) {(float)rotateObj->_curAngle, (float)rotateObj->_tarAngle};
}
static Maze* GetMazeInfo(RotateObj* rotateObj) {
	return rotateObj->_mazeInfo;
}

// for now, I just pass in the args for the sake of using the function pointer
static void RotateObj_Move(Obj* obj, MoveArgs args) {
	RotateObj* rotateObj = (RotateObj*)obj;

	if (obj->_isMoving || (rotateObj->_lastRotateTime > 0 && GetTime() - rotateObj->_lastRotateTime < rotateObj->_rotateInterval))
		return;

	obj->_isMoving = 1;
	
	int rotationTypes[] = {-180, -90, 90, 180};
	size_t size = sizeof(rotationTypes) / sizeof(rotationTypes[0]);

	int addAngle = rotationTypes[GetRandomValue(0, size - 1)];
	rotateObj->_speed = addAngle / obj->_movingDuration;
	rotateObj->_dir = 1 - 2 * (rotateObj->_speed < 0);	// _dir == 1 -> _curAngle increases; dir == -1 -> _curAngle decreases
	rotateObj->_tarAngle = rotateObj->_curAngle + addAngle;
	rotateObj->_lastRotateTime = GetTime();
}
static void RotateObj_Draw(Obj* obj, Vector2 alterV) {
	DrawTextureEx(obj->_rt.texture, alterV, 0.0f, 1.0f, WHITE);
}

static void RotateObj_Update(Obj* obj) {
	RotateObj* rotateObj = (RotateObj*)obj;

	if (obj->_isMoving && (int)rotateObj->_curAngle * rotateObj->_dir >= (int)rotateObj->_tarAngle * rotateObj->_dir) {
		int cycleAngle = (int)rotateObj->_tarAngle;
		if (cycleAngle < -360)
			cycleAngle += 360;
		else if (cycleAngle >= 360)
			cycleAngle -= 360;
		rotateObj->_curAngle = rotateObj->_tarAngle = cycleAngle;

		rotateObj->_speed = 0.0;
		obj->_isMoving = 0;
	}
	else {
        rotateObj->_curAngle += GetFrameTime() * rotateObj->_speed;
	}
}

static void RotateObj_Resize(Obj* obj, int width, int height) {
	int newWidth = (width != -1) ? width : obj->_rt.texture.width;
	int newHeight = (height != -1) ? height : obj->_rt.texture.height;
	bool changed = (newWidth != obj->_rt.texture.width || newHeight != obj->_rt.texture.height);

	if (changed) {
		RotateObj* rotateObj = (RotateObj*)obj;

		UnloadRenderTexture(obj->_rt);
		obj->_rt = LoadRenderTexture(newWidth, newHeight);

		BeginTextureMode(obj->_rt);
			DrawMaze(*rotateObj);
		EndTextureMode();
	}
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
	free(rotateObj->_mazeInfo->maze);
	rotateObj->_mazeInfo->maze = NULL;
	free(rotateObj->_mazeInfo);
	rotateObj->_mazeInfo = NULL;

	free(rotateObj);
}


static void LoadMaze (RotateObj* rotateObj, MazeLoadingArgs args) {
	if (args.type == GENERATE) {
		int inputCol = (int)args.data.mazeSize.x;
		int inputRow = (int)args.data.mazeSize.y;
		
		GenerateMaze(rotateObj->_mazeInfo, inputCol, inputRow);
	}
	else if (args.type == LOAD_FILE) {
		LoadMap(rotateObj->_mazeInfo, args.data.file);
	}
}
	
// remember to add file loading here (maze loading)
Obj* New_RotateObj(MazeLoadingArgs args, double interval, double movingDuration, int width, int height) {
	RotateObj* obj = (RotateObj*)malloc(sizeof(RotateObj));
	if (NULL == obj)
		return NULL;

	obj->base = (Obj){
		0,
		movingDuration,
		LoadRenderTexture(width, height),

		RotateObj_GetPos,
		Obj_GetSize,
		RotateObj_Move,
		RotateObj_Draw,
		RotateObj_Update,
		RotateObj_Resize,
		RotateObj_Free,
	};

	obj->_mazeInfo = (Maze*)malloc(sizeof(Maze));
	if (obj->_mazeInfo != NULL)
		LoadMaze(obj, args);
	BeginTextureMode(obj->base._rt);
		DrawMaze(*obj);
	EndTextureMode();

	obj->_dir = 0;
	obj->_speed = obj->_curAngle = obj->_tarAngle = 0.0;
	obj->_lastRotateTime = -1;
	obj->_rotateInterval= interval;
	obj->GetMazeInfo = GetMazeInfo;

	return (Obj*)obj;
}

// ******* MAZE'S SECTION ENDS *******
