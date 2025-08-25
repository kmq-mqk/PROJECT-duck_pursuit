#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "raylib.h"
#include "object.h"

//typedef struct {
//    bool visited;
//    bool topWall, bottomWall, leftWall, rightWall;
//} Cell;
//
//typedef struct {
//	int col, row;
//	Cell** maze;
//} Maze;

//typedef struct {
//    bool isMoving;
//    int dirX, dirY;
//    Vector2 speed;
//    Vector2 curPos;
//    Position tarPos;
//} MobiObj;


// ---------- FOR MAP GENERATING
void GenerateMaze(Maze* mazeInfo, int inputCol, int inputRow);
void InitializeMaze(Cell*** maze, int inputCol, int inputRow);
void CreateMaze(Cell*** maze,int col, int row, int x, int y);
void AddLoops(Cell*** maze,int col, int row, int loopCount);

// ---------- FOR MAP READ/WRITE
void ReadTxt(Cell***, int j, char* line, int n);
void LoadMap(Maze*, char* fileName);
void WriteMap(const Maze, char* fileName);

void ViewMap(char* fileName);
void MapInterface();

#endif
