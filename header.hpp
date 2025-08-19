#pragma once
#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>

#include "map.hpp"
#include "render.hpp"
#include "logic.hpp"
#include "raygui.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIZE 60
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)
#define LIGHT_DISTANCE 2


typedef enum GameScreen{
    OPENING, GAMEPLAY, ENDING
}GameScreen;


void GameStart();

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

