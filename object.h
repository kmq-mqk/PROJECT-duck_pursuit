#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "raylib.h"
#include <stdarg.h>


// -------- DON'T USE DIRECTLY THE VARIABLES WITH LEADING '_' -------

typedef struct Cell Cell;
typedef struct Maze Maze;
typedef struct MoveArgs MoveArgs;

typedef struct Obj Obj;

struct Obj {
    bool _isMoving;
	double _movingDuration;
	RenderTexture _rt;

	// METHODS BEGIN *******
	void (*Move)(Obj*, const MoveArgs);
	void (*Draw)(Obj*, Vector2 renderPos);
	void (*Update)(Obj*);
	void (*Resize)(Obj*, int width, int height);
	void (*Free)(Obj*);
	// METHODS END *******
};

typedef struct {
	Obj _base;
	Image _sprite;
    int _dirX, _dirY;
    Vector2 _speed;
    Vector2 _curPos, _tarPos;
} MobiObj;

typedef struct {
	Obj _base;
	int _dir;
	double _speed;
	double _curAngle, _tarAngle;
	Maze* _mazeInfo;
} RotateObj;

Obj* New_MobiObj(double movingDuration, int width , int height);
Obj* New_RotateObj(double movingDuration, int width, int height);

// I don't think I will keep this
Cell** LoadMaze(char mode, ...);


#ifdef __cplusplus
}
#endif

#endif
