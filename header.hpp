#pragma once
#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CELL_SIZE 400
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)
#define LIGHT_DISTANCE 2


typedef struct {
    int x, y;
} Position;

typedef struct {
    bool visited;
    bool topWall, bottomWall, leftWall, rightWall;
} Cell;

typedef enum GameScreen{
    OPENING, GAMEPLAY, ENDING
}GameScreen;



using namespace std;

class Penguin {
    public: 
        Penguin();
        ~Penguin();
        void Draw();
        void Update();
    private:
        Vector2 position;
        Texture2D texture; // code new struct Sprite if neccesary 
};

