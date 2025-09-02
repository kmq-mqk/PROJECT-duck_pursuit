#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "render.hpp"

#include "raylib.h"
#include <stdarg.h>
#include <stdio.h>


// -------- DON'T USE DIRECTLY THE VARIABLES WITH LEADING '_' -------

typedef enum {
	GENERATE,
	LOAD_FILE
} LoadType;

typedef struct {
	LoadType type;
	int loop;
	union {
		Vector2 mazeSize;
		char* file;
	} data;
} MazeLoadingArgs;


typedef struct {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
} Cell;

typedef struct {
	int col, row;
	Cell** maze;	// _maze[col][row]
} Maze;


typedef struct {
	Maze* mazeInfo;
	int dx, dy;
	float rotation;
} MoveArgs;

typedef struct Obj Obj;

struct Obj {
    bool _isMoving;
	double _movingDuration;
	int _width, _height;
	Texture _tex;
	float _scale;

	// METHODS BEGIN *******
	bool (*IsMoving)(Obj*);
	Vector2 (*GetPos)(Obj*);
	Vector2 (*GetSize)(Obj*);
	float (*GetAngleDegree)(Obj*);
	void (*ChangeAngleDegree)(Obj*, float rotation);
	void (*Move)(Obj*, const MoveArgs);
	void (*Draw)(Obj*);
	void (*Update)(Obj*);
	void (*Resize)(Obj*, int width, int height);
	void (*Free)(Obj*);
	// METHODS END *******
};

typedef struct {
	Obj base;
	float _rotation;		// degree (NOT RADIAN)
    int _dirX, _dirY;
    Vector2 _speed;
    Vector2 _curPos, _tarPos;
} MobiObj;

typedef struct RotaObj {
	Obj base;
	int _dir;
	double _speed;
	double _lastRotateTime, _rotateInterval;
	RenderTexture _rt;
	double _curAngle, _tarAngle;
	Maze* _mazeInfo;

	Maze* (*GetMazeInfo)(struct RotaObj*);
} RotaObj;

Obj* New_MobiObj(Vector2 location, const char* spriteFile, double movingDuration, int width , int height);
Obj* New_RotaObj(MazeLoadingArgs args, double interval, double movingDuration, int width, int height);


typedef struct RenderList RenderList;

struct RenderList {
	size_t mobiCount;
	MobiObj** mobiList;
	size_t rotaCount;
	RotaObj** rotaList;
};


void Free(RenderList* list);

#ifdef __cplusplus
}
#endif

#endif
