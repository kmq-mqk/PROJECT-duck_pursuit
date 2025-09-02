#include "object.h"
#include "map.hpp"
#include "render.hpp"

#include <stdio.h>
#include <stdlib.h>


static bool Obj_IsMoving(Obj* obj) {
	return obj->_isMoving;
}

// mobi: max size of sprite
// rota: max size
static Vector2 Obj_GetSize(Obj* obj) {
	return (Vector2){(float)obj->_width, (float)obj->_height};
}

// ******* PLAYER'S SECTION BEGINS *******

static Vector2 Mobi_GetPos(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;
	return mobiObj->_curPos;
}

static float Mobi_GetAngleDegree(Obj* obj) {
	MobiObj* mobi = (MobiObj*)obj;
	return mobi->_rotation;
}

static void Mobi_ChangeAngleDegree(Obj* obj, float rotation) {
	MobiObj* mobi = (MobiObj*)obj;
	mobi->_rotation = rotation;
}

// args: Obj* mobiObj, Maze mazeInfo, int dx, int dy
static void Mobi_Move(Obj* obj, const MoveArgs args) {
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

static void Mobi_Draw(Obj* obj) {
    MobiObj* mobi = (MobiObj*)obj;

    float cellX = mobi->_curPos.x * obj->_width;
    float cellY = mobi->_curPos.y * obj->_height;

    float spriteW = mobi->base._tex.width * mobi->base._scale;
    float spriteH = mobi->base._tex.height * mobi->base._scale;

//    Vector2 renderPos = {
//        cellX + (obj->_width  - spriteW) / 2.0f,
//        cellY + (obj->_height - spriteH) / 2.0f
//    };

	Vector2 renderPos = {
		cellX + obj->_width / 2.0f,
		cellY + obj->_height / 2.0f
	};

//    DrawTextureEx(mobi->base._tex, renderPos, 45.0f, mobi->base._scale, WHITE);
	DrawTextByCenter(mobi->base._tex, renderPos, mobi->_rotation, mobi->base._scale, WHITE, 0);
}

static void Mobi_Update(Obj* obj) {
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

static void Mobi_Resize(Obj* obj, int width, int height) {	// input cellSize
	MobiObj* mobi = (MobiObj*)obj;

	//	WE CHANGE THE VALUE OF SCALE INSTEAD
	obj->_width = width;
	obj->_height = height;

//	int minEdge = (obj->_width < obj->_height) ? obj->_width : obj->_height;
//	int minText = (mobi->base._tex.width < mobi->base._tex.height) ? mobi->base._tex.width : mobi->base._tex.height; 
//	mobi->base._scale = (float)(minEdge - 5.0f) / minText;

	int maxEdge = (obj->_width > obj->_height) ? obj->_width : obj->_height;
	int maxText = (mobi->base._width > mobi->base._height) ? mobi->base._width : mobi->base._height;
	mobi->base._scale = (float)(maxEdge - 5.0f) / maxText;
}

static void Mobi_Free(Obj* obj) {
	MobiObj* mobiObj = (MobiObj*)obj;

	if (IsTextureValid(mobiObj->base._tex))
		UnloadTexture(mobiObj->base._tex);

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
		(Texture){0},
		0.0f,

		Obj_IsMoving,
		Mobi_GetPos,
		Obj_GetSize,
		Mobi_GetAngleDegree,
		Mobi_ChangeAngleDegree,
		Mobi_Move,
		Mobi_Draw,
		Mobi_Update,
		Mobi_Resize,
		Mobi_Free,
	};
	
	Image img = LoadImage(spriteFile);
	mobiObj->base._tex = LoadTextureFromImage(img);
	SetTextureWrap(mobiObj->base._tex, TEXTURE_WRAP_CLAMP);
	UnloadImage(img);

	mobiObj->_rotation = 0.0f;
	Mobi_Resize((Obj*)mobiObj, width, height);

	mobiObj->_dirX = mobiObj->_dirY = 0;
	mobiObj->_speed =  (Vector2){0, 0};
	mobiObj->_curPos = mobiObj->_tarPos = location;

	return (Obj*)mobiObj;
}

// ******* PLAYER'S SECTION ENDS *******


// ******* MAZE'S SECTION BEGINS *******

static void DrawMaze(RotaObj* rotateObj) {
	int col = rotateObj->_mazeInfo->col;
	int row = rotateObj->_mazeInfo->row;
	Cell** maze = rotateObj->_mazeInfo->maze;
//	float cellWidth = (float)rotateObj->base._width;
//	float cellHeight = (float)rotateObj->base._height;

	float cellWidth = MeasureCellSize((Vector2){rotateObj->base._width, rotateObj->base._height}, (Vector2){col, row});
	float cellHeight = cellWidth;

	Vector2 adjust = {5, 5};
	rotateObj->_rt = LoadRenderTexture(col * cellWidth + adjust.x, row * cellHeight + adjust.y);
	
	BeginTextureMode(rotateObj->_rt);
	    for (int x = 0; x < col; x++) {
	        for (int y = 0; y < row; y++) {
	            float posX = cellWidth * x + adjust.x / 2.0f;
	            float posY = cellHeight * y + adjust.y / 2.0f;
	
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
	EndTextureMode();

//	BeginDrawing();
//		ClearBackground(BLACK);
//		DrawTexture(rt.texture, 0, 0, WHITE);
//	EndDrawing();

	rotateObj->base._tex = rotateObj->_rt.texture;
}

static Vector2 Rota_GetPos(Obj* obj) {
	RotaObj* rotateObj = (RotaObj*)obj;
	return (Vector2) {(float)rotateObj->_curAngle, (float)rotateObj->_tarAngle};
}
static Maze* GetMazeInfo(RotaObj* rotateObj) {
	return rotateObj->_mazeInfo;
}

static float Rota_GetAngleDegree(Obj* obj) {
	RotaObj* rota = (RotaObj*)obj;
	return rota->_curAngle;
}

// for now, I just pass in the args for the sake of using the function pointer
static void Rota_Move(Obj* obj, MoveArgs args) {
	RotaObj* rotateObj = (RotaObj*)obj;

	if (rotateObj->_lastRotateTime < 0) {
		rotateObj->_lastRotateTime = GetTime();
		return;
	}

	if (obj->_isMoving || GetTime() - rotateObj->_lastRotateTime < rotateObj->_rotateInterval)
		return;

	float addAngle = args.rotation;

	obj->_isMoving = 1;
	
	rotateObj->_speed = addAngle / obj->_movingDuration;
	rotateObj->_dir = 1 - 2 * (rotateObj->_speed < 0);	// _dir == 1 -> _curAngle increases; dir == -1 -> _curAngle decreases
	rotateObj->_tarAngle = rotateObj->_curAngle + addAngle;
	rotateObj->_lastRotateTime = GetTime();
}
static void Rota_Draw(Obj* obj) {
	Rectangle src = {0, 0, obj->_tex.width, -obj->_tex.height};
//	Rectangle dest = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, obj->_tex.width * obj->_scale, obj->_tex.height * obj->_scale};
	Rectangle dest = {0, 0, obj->_tex.width * obj->_scale, obj->_tex.height * obj->_scale};
//	Vector2 origin = {obj->_scale * obj->_tex.width / 2.0f, obj->_scale * obj->_tex.height / 2.0f};
	Vector2 origin = {0, 0};
	
	DrawTexturePro(obj->_tex, src, dest, origin, ((RotaObj*)obj)->_curAngle, WHITE);
}

static void Rota_Update(Obj* obj) {
	RotaObj* rotateObj = (RotaObj*)obj;

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

static void Rota_Resize(Obj* obj, int width, int height) {		// width is usually stands for screenWidth, similar to height.
	RotaObj* rota = (RotaObj*)obj;

	Maze* mazeInfo = rota->GetMazeInfo(rota);
	double cell = MeasureCellSize((Vector2){width, height}, (Vector2){mazeInfo->col, mazeInfo->row});

//	int minPrevCell = (obj->_width < obj->_height) ? obj->_width : obj->_height;
//	obj->_scale = 
//	obj->_width = cell;
//	obj->_height = cell;

	int newW = mazeInfo->col * cell;
	int newH = mazeInfo->row * cell;
	int maxNew = (newW > newH) ? newH : newH;

	int maxTex = (obj->_width > obj->_height) ? obj->_tex.width : obj->_tex.height;
	obj->_scale = (float)maxNew / maxTex;
	obj->_width = newW;
	obj->_height = newH;
}

static void Rota_Free(Obj* obj) {
	RotaObj* rotateObj = (RotaObj*)obj;

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

	UnloadRenderTexture(((RotaObj*)obj)->_rt);

	free(rotateObj);
}


static void LoadMaze (RotaObj* rota, MazeLoadingArgs args) {
	if (args.type == GENERATE) {
		int inputCol = (int)args.data.mazeSize.x;
		int inputRow = (int)args.data.mazeSize.y;
		int loop = args.loop;
		
		GenerateMaze(rota->_mazeInfo, loop, inputCol, inputRow);
	}
	else if (args.type == LOAD_FILE) {
		LoadMap(rota->_mazeInfo, args.data.file);
	}

//	int curMonitor = GetCurrentMonitor();
//	int monitorW = GetMonitorWidth(curMonitor);
//	int monitorH = GetMonitorHeight(curMonitor);
//	int maxMonitorEdge = (monitorW > monitorH) ? monitorW : monitorH;
//	rota->base._width = rota->base._height = maxMonitorEdge;
//
	int screenH = GetScreenHeight();
	int screenW = GetScreenWidth();
	int maxEdge = (screenH > screenW) ? screenH : screenW;
	rota->base._width = rota->base._height = maxEdge;

	DrawMaze(rota);
}
	
// remember to add file loading here (maze loading)
Obj* New_RotaObj(MazeLoadingArgs args, double interval, double movingDuration, int width, int height) {
	RotaObj* obj = (RotaObj*)malloc(sizeof(RotaObj));

	if (NULL == obj)
		return NULL;

	obj->_mazeInfo = (Maze*)malloc(sizeof(Maze));
	if (obj->_mazeInfo == NULL)
		perror("Not enough memory");

	double cell = MeasureCellSize((Vector2){width, height}, args.data.mazeSize);

	obj->_dir = 0;
	obj->_speed = obj->_curAngle = obj->_tarAngle = 0.0;
	obj->_lastRotateTime = -1;
	obj->_rotateInterval= interval;
	obj->GetMazeInfo = GetMazeInfo;

	obj->base = (Obj){
		0,
		movingDuration,
		0, 0,
		(Texture){0},
		0.0f,

		Obj_IsMoving,
		Rota_GetPos,
		Obj_GetSize,
		Rota_GetAngleDegree,
		NULL,
		Rota_Move,
		Rota_Draw,
		Rota_Update,
		Rota_Resize,
		Rota_Free,
	};

	LoadMaze(obj, args);

	Rota_Resize((Obj*)obj, width, height);

	return (Obj*)obj;
}

// ******* MAZE'S SECTION ENDS *******

void Free(RenderList* list) {
	size_t mobiCnt = list->mobiCount;
	size_t rotaCnt = list->rotaCount;
	MobiObj** mobiLs = list->mobiList;
	RotaObj** rotaLs = list->rotaList;

	for (size_t cnt = 0; cnt < mobiCnt; cnt++) {
		mobiLs[cnt]->base.Free((Obj*)mobiLs[cnt]);
	}
	for (size_t cnt = 0; cnt < rotaCnt; cnt++) {
		rotaLs[cnt]->base.Free((Obj*)rotaLs[cnt]);
	}
}
