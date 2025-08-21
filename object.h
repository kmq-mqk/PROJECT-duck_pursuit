#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "raylib.h"
#include <stdarg.h>


// -------- DON'T USE DIRECTLY THE VARIABLES WITH LEADING '_' -------

typedef struct {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
} Cell;

typedef struct {
	int _col, _row;
	Cell** _maze;
} Maze;

typedef struct Obj Obj;

struct Obj {
    bool _isMoving;
	double _movingDuration;
	int _width, _height;	// for size of players or size of maze's cell

	// METHODS BEGIN *******
	void (*Move)(Obj*, ...);
	void (*Draw)(Obj*);
	void (*Update)(Obj*);
	void (*Free)(Obj*);
	// METHODS END *******
};

typedef struct {
	Obj _base;
    int _dirX, _dirY;
    Vector2 _speed;
    Vector2 _curPos, _tarPos;
} MobiObj;

typedef struct {
	Obj _base;
	int _dir;
	double _speed;
	double _curAngle, _tarAngle;
	RenderTexture2D _texture;
	Maze _mazeInfo;
} RotateObj;

Obj* New_MobiObj(double movingDuration, int width , int height);
Obj* New_RotateObj(double movingDuration, int width, int height);

Cell** LoadMaze(char mode, ...);


#ifdef __cplusplus
}
#endif

#endif
