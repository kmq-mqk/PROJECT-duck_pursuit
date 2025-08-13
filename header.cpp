#include "header.hpp"

Penguin::Penguin(){


}

Penguin::~Penguin() {

}

void Penguin::Draw() {

}

void Penguin::Update() {
    
}


#include <stdlib.h>

extern bool gameWon;
extern RenderTexture2D mazeTexture;
extern double lastAutoRotateTime;
extern float autoRotateInterval;

extern int col;
// variables that need to be freed
extern Cell** maze;

void GameStart(){
    GameScreen currentScreen = OPENING;
    InitAudioDevice();

    bool init = false;
   
    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
                BeginDrawing();
                    ClearBackground(SKYBLUE);
                    Rectangle playButton = {(SCREEN_WIDTH - MeasureText("PLAY", 100)) / 2, 500, MeasureText("PLAY", 100), 40};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SKYBLUE);
                    DrawText("DUCK PURSUIT", 100, 300, 80, ORANGE);
                    if(GuiButton(playButton, "PLAY")){
                        gameWon = false;
                        currentScreen = GAMEPLAY;   
                        if (mazeTexture.id != 0) {
                            UnloadRenderTexture(mazeTexture);
                        }
                        
                        mazeTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
                        
                        GenerateMaze(4, 10);
                        AddLoops(5);
                    }
                EndDrawing();
            }break;
            case GAMEPLAY:
            {
                // 3 LINES BELOW ARE IMPORTANT  !!!
                if (!init) {
                    lastAutoRotateTime = GetTime();
                    autoRotateInterval = 2.0f;
                    init = true;
                }

				InputMove();
				// there will be a function below to check and change rendering values in few foreseeable days
				double cellSize = MeasureCellSize();   // now, the function is only for putting the maze in the center of the screen 
				Vector2 alterVec = MeasureAlterVec(cellSize);
				Render(alterVec, cellSize, 0.5);

                if (gameWon){
                    Free();
                    currentScreen = ENDING;
                    UnloadRenderTexture(mazeTexture);
                }
            }break;

            case ENDING:
            {
                BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("You win!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 30, DARKGREEN);
                    Rectangle playAgainButton = {(SCREEN_WIDTH - MeasureText("PLAY AGAIN", 100)) / 2, 500, MeasureText("PLAY AGAIN", 100), 40};
                    if (GuiButton(playAgainButton, "PLAY AGAIN"))
                        currentScreen = OPENING;
                EndDrawing();
                init = false;
            }break;
            default: break;
        }
    }

    CloseAudioDevice();
}

void Free() {
    if (maze == NULL)
        return;

    for (int i = 0; i < col; i++) {
        free(maze[i]);
        maze[i] = NULL;
    }
    free(maze);
    maze = NULL;
}
