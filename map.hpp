#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <raylib.h>


typedef struct {
    int x, y;
} Position;

typedef struct {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
} Cell;

typedef struct {
    bool isMoving;
    int dirX, dirY;
    Vector2 speed;
    Vector2 curPos;
    Position tarPos;
} MobiObj;

// extern Cell** maze;    // maze[col][row]
// extern int col;    // Ox
// extern int row;    // Oy
// extern Position goal;
// extern MobiObj player;

// ---------- FOR MAP GENERATING
void GenerateMaze(int inputCol, int inputRow);
void InitializeMaze();
void CreateMaze(int x, int y);
void AddLoops(int loopCount);

// ---------- FOR MAP READ/WRITE
void ReadTxt(int j, char* line, int n);
void LoadMap(char* fileName);
void WriteMap(char* fileName);

#endif
