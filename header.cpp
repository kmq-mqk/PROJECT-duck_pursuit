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

    // OPENING background:
    Image *background = new Image {LoadImage("resources/background.png")};
    ImageResize(background, SCREEN_WIDTH, SCREEN_HEIGHT);
    Texture2D bgTexture = LoadTextureFromImage(*background);

    Image *playButton = new Image {LoadImage("resources/playbutton.png")};
    ImageResize(playButton, 180, 60);
    Texture2D playButtonTexTure = LoadTextureFromImage(*playButton);
     
    Image *playButton2 = new Image {LoadImage("resources/playbutton2.png")};
    ImageResize(playButton2, 180, 60);
    Texture2D playButtonTexTure2 = LoadTextureFromImage(*playButton2);
    //-------------------
    //ENDING background:
    Image *background2 = new Image {LoadImage("resources/ending.png")};
    ImageResize(background2, SCREEN_WIDTH, SCREEN_HEIGHT);
    Texture2D bgTexture2 = LoadTextureFromImage(*background2);
    
    Image *endButton = new Image {LoadImage("resources/endbutton.png")};
    ImageResize(endButton, 180, 60);
    Texture2D endButtonTexTure = LoadTextureFromImage(*endButton);

    Image *endButton2 = new Image {LoadImage("resources/endbutton2.png")};
    ImageResize(endButton2, 180, 60);
    Texture2D endButtonTexTure2 = LoadTextureFromImage(*endButton2);
    //-------------------

    bool init = false;
   
    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
                BeginDrawing();
                    DrawTextureV(bgTexture, {0,0}, WHITE);
                    DrawTexture(playButtonTexTure, SCREEN_WIDTH/2 - playButtonTexTure.width/2, SCREEN_HEIGHT/2 - playButtonTexTure.height/2 + 150, WHITE);
                    Rectangle playButton_Rec = {SCREEN_WIDTH/2 - 180/2, SCREEN_HEIGHT/2 - 60/2 + 150, 180, 60};

                    if(CheckCollisionPointRec(GetMousePosition(), playButton_Rec)){
                        DrawTexture(playButtonTexTure2, SCREEN_WIDTH/2 - playButtonTexTure2.width/2, SCREEN_HEIGHT/2 - playButtonTexTure2.height/2 + 150, WHITE);
                        if(IsMouseButtonPressed(0)){
                            //go to GAMEPLAY
                            gameWon = false;
                            currentScreen = GAMEPLAY;   
                            if (mazeTexture.id != 0) {
                                UnloadRenderTexture(mazeTexture);
                            }
                            
                            mazeTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
                            
                            GenerateMaze(4, 10);
                            AddLoops(5);
                        }
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
                    DrawTextureV(bgTexture2, {0,0}, WHITE);
                    DrawTexture(endButtonTexTure, SCREEN_WIDTH/2 - endButtonTexTure.width/2, SCREEN_HEIGHT/2 - endButtonTexTure.height/2 + 250, WHITE);
                    Rectangle endButton_Rec = {SCREEN_WIDTH/2 - 180/2, SCREEN_HEIGHT/2 - 60/2 + 250, 180, 60};
                    
                    if (CheckCollisionPointRec(GetMousePosition(), endButton_Rec)){
                        DrawTexture(endButtonTexTure2, SCREEN_WIDTH/2 - endButtonTexTure2.width/2, SCREEN_HEIGHT/2 - endButtonTexTure2.height/2 + 250, WHITE);  
                        if(IsMouseButtonPressed(0))
                            currentScreen = OPENING;
                    }
                EndDrawing();
                init = false;
            }break;
            default: break;
        }
    }

    UnloadTexture(bgTexture);
    UnloadTexture(playButtonTexTure);
    UnloadTexture(playButtonTexTure2);
    UnloadTexture(bgTexture2);
    UnloadTexture(endButtonTexTure);
    UnloadTexture(endButtonTexTure2);


    delete background;
    delete playButton;
    delete playButton2;

    delete background2;
    delete endButton;
    delete endButton2;

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
