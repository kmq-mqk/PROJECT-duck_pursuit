#include "object.h"
#include "map.hpp"
#include "render.hpp"

#include <stdio.h>
#include <stdlib.h>

// mobi: max size of sprite
// rota: max size of cell
static Vector2 Obj_GetSize(Obj* obj) {
	return (Vector2){(float)obj->_width, (float)obj->_height};
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

	if (mobiObj->base._isMoving)
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

static void MobiObj_Draw(Obj* obj) {
    MobiObj* mobi = (MobiObj*)obj;

    float cellX = mobi->_curPos.x * obj->_width;
    float cellY = mobi->_curPos.y * obj->_height;

    float spriteW = mobi->_texture.width * mobi->_scale;
    float spriteH = mobi->_texture.height * mobi->_scale;

    Vector2 renderPos = {
        cellX + (obj->_width  - spriteW) / 2.0f,
        cellY + (obj->_height - spriteH) / 2.0f
    };

    DrawTextureEx(mobi->_texture, renderPos, 0.0f, mobi->_scale, WHITE);
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

static void MobiObj_Resize(Obj* obj, int width, int height) {	// input cellSize
	MobiObj* mobi = (MobiObj*)obj;

	//	WE CHANGE THE VALUE OF SCALE INSTEAD
	obj->_width = width;
	obj->_height = height;

	int minEdge = (obj->_width < obj->_height) ? obj->_width : obj->_height;
	int minText = (mobi->_texture.width < mobi->_texture.height) ? mobi->_texture.width : mobi->_texture.height; 
	mobi->_scale = (float)(minEdge - 5.0f) / minText;
}

static void MobiObj_Free(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;

	if (IsTextureValid(mobiObj->_texture))
		UnloadTexture(mobiObj->_texture);

	free(mobiObj);
}

// remember to add image loading
Obj* New_MobiObj(Vector2 location, const char* spriteFile, double movingDuration, int width, int height) {
	MobiObj* mobiObj = (MobiObj*)malloc(sizeof(MobiObj));

	if (!mobiObj)
		return NULL;

	mobiObj->base = (Obj){
		0,
		movingDuration,
		width, height,

		MobiObj_GetPos,
		Obj_GetSize,
		MobiObj_Move,
		MobiObj_Draw,
		MobiObj_Update,
		MobiObj_Resize,
		MobiObj_Free,
	};
	
	Image img = LoadImage(spriteFile);
	mobiObj->_texture = LoadTextureFromImage(img);
	SetTextureWrap(mobiObj->_texture, TEXTURE_WRAP_CLAMP);
	UnloadImage(img);

	MobiObj_Resize((Obj*)mobiObj, width, height);

	mobiObj->_dirX = mobiObj->_dirY = 0;
	mobiObj->_speed =  (Vector2){0, 0};
	mobiObj->_curPos = mobiObj->_tarPos = location;

	return (Obj*)mobiObj;
}

// ******* PLAYER'S SECTION ENDS *******


// ******* MAZE'S SECTION BEGINS *******

static void DrawMaze(RotateObj rotateObj) {
	int col = rotateObj._mazeInfo->col;
	int row = rotateObj._mazeInfo->row;
	Cell** maze = rotateObj._mazeInfo->maze;
	float cellWidth = (float)rotateObj.base._width;
	float cellHeight = (float)rotateObj.base._height;

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            float posX = cellWidth * x;
            float posY = cellHeight * y;

            if (maze[x][y].topWall) {
				Vector2 start = {posX, posY};
				Vector2 end = {posX + cellWidth, posY};
			   	DrawLineEx(start, end, 2.0f, WHITE);
			}
            if (maze[x][y].rightWall) {
				Vector2 start = {posX + cellWidth, posY};
				Vector2 end = {posX + cellWidth, posY + cellHeight};
			   	DrawLineEx(start, end, 2.0f, WHITE);
			}
            if (maze[x][y].leftWall) {
				Vector2 start = {posX, posY};
				Vector2 end = {posX, posY + cellHeight};
			   	DrawLineEx(start, end, 2.0f, WHITE);
			}
            if (maze[x][y].bottomWall) {
				Vector2 start = {posX, posY + cellHeight};
				Vector2 end = {posX + cellWidth, posY + cellHeight};
			   	DrawLineEx(start, end, 2.0f, WHITE);
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

	if (rotateObj->_lastRotateTime < 0) {
		rotateObj->_lastRotateTime = GetTime();
		return;
	}

	if (obj->_isMoving || GetTime() - rotateObj->_lastRotateTime < rotateObj->_rotateInterval)
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
static void RotateObj_Draw(Obj* obj) {
	RotateObj* rota = (RotateObj*)obj;
	DrawMaze(*rota);
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

static void RotateObj_Resize(Obj* obj, int width, int height) {		// width is usually stands for screenWidth, similar to height.
	RotateObj* rota = (RotateObj*)obj;

	Maze* mazeInfo = rota->GetMazeInfo(rota);
	double cell = MeasureCellSize((Vector2){width, height}, (Vector2){mazeInfo->col, mazeInfo->row});

	obj->_width = cell;
	obj->_height = cell;
}

static void RotateObj_Free(Obj* obj) {
	RotateObj* rotateObj = (RotateObj*)obj;

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
		int loop = args.loop;
		
		GenerateMaze(rotateObj->_mazeInfo, loop, inputCol, inputRow);
	}
	else if (args.type == LOAD_FILE) {
		LoadMap(rotateObj->_mazeInfo, args.data.file);
	}
}
	
// remember to add file loading here (maze loading)
Obj* New_RotateObj(MazeLoadingArgs args, double interval, double movingDuration, int width, int height) {
	RotateObj* obj = (RotateObj*)malloc(sizeof(RotateObj));

	double cell = MeasureCellSize((Vector2){width, height}, args.data.mazeSize);
	
	if (NULL == obj)
		return NULL;

	obj->base = (Obj){
		0,
		movingDuration,
		cell, cell,

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

	obj->_dir = 0;
	obj->_speed = obj->_curAngle = obj->_tarAngle = 0.0;
	obj->_lastRotateTime = -1;
	obj->_rotateInterval= interval;
	obj->GetMazeInfo = GetMazeInfo;

	return (Obj*)obj;
}

// ******* MAZE'S SECTION ENDS *******

void Free(RenderList* list) {
	size_t mobiCnt = list->mobiCount;
	size_t rotaCnt = list->rotaCount;
	MobiObj** mobiLs = list->mobiList;
	RotateObj** rotaLs = list->rotaList;

	for (size_t cnt = 0; cnt < mobiCnt; cnt++) {
		mobiLs[cnt]->base.Free((Obj*)mobiLs[cnt]);
	}
	for (size_t cnt = 0; cnt < rotaCnt; cnt++) {
		rotaLs[cnt]->base.Free((Obj*)rotaLs[cnt]);
	}
}
