#pragma once
#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <stdlib.h>
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIZE 60
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)
#define LIGHT_DISTANCE 2


extern bool gameWon;
extern RenderTexture2D mazeTexture;
extern double lastAutoRotateTime;
extern float autoRotateInterval;


// typedef struct {
//     int x, y;
// } Position;

// typedef struct {
//     bool visited;
//     bool topWall, bottomWall, leftWall, rightWall;
// } Cell;

typedef enum GameScreen{
    OPENING, GAMEPLAY, ENDING
}GameScreen;


void GameStart(){
    GameScreen currentScreen = OPENING;

    SetTargetFPS(60);
    InitAudioDevice();
    BeginDrawing();
    ClearBackground(BLACK);
   
    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
                Rectangle playButton = {(SCREEN_WIDTH - MeasureText("PLAY", 100)) / 2, 500, MeasureText("PLAY", 100), 40};
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SKYBLUE);
                DrawText("DUCK PURSUIT", 100, 300, 80, ORANGE);

                if(GuiButton(playButton, "PLAY")){
                    gameWon = false;
                    currentScreen = GAMEPLAY;
                    mazeTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
                    InitializeMaze();
                    GenerateMaze(0, 0);
                    player.x = 0, player.y = 0;
                    goal.x = GRID_WIDTH - 1;
                    goal.y = GRID_HEIGHT - 1;
                }
            }break;
            case GAMEPLAY:
            {
                // 3 LINES BELOW ARE IMPORTANT  !!!
                lastAutoRotateTime = GetTime();
                autoRotateInterval = 5.0f;
                // mazeTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

                InputMove();
                Render(CELL_SIZE, 1);

                if (gameWon){
                    currentScreen = ENDING;
                    UnloadRenderTexture(mazeTexture);
                }
            }break;

            case ENDING:
            {
                ClearBackground(BLACK);
                DrawText("You win!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 30, DARKGREEN);
                Rectangle playAgainButton = {(SCREEN_WIDTH - MeasureText("PLAY AGAIN", 100)) / 2, 500, MeasureText("PLAY AGAIN", 100), 40};
                if (GuiButton(playAgainButton, "PLAY AGAIN"))
                currentScreen = OPENING;
            }break;
            default: break;
        }


        // Vẽ giao diện

        
    //     switch(currentScreen)
    //     {
    //         case OPENING:
    //         {
    //         }break;

    //         case GAMEPLAY:
    //         {

    //         }break;

    //         case ENDING:
    //         {

    //             Rectangle playAgainButton = {(SCREEN_WIDTH - MeasureText("PLAY AGAIN", 100)) / 2, 500, MeasureText("PLAY AGAIN", 100), 40};
    //             int button2 = GuiButton(playAgainButton, "PLAY AGAIN");
    //         }break;
    //         default: break;
    //     }

    
        EndDrawing();

    }

    CloseAudioDevice();
}


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

