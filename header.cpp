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

extern int screenWidth, screenHeight;
extern MobiObj player;
extern Vector2 alterVec;
extern double cellSize;


void GameStart(){
    GameScreen currentScreen = OPENING;
    InitAudioDevice();

    // OPENING background:
    Image *background = new Image {LoadImage("resources/image/background.png")};
	ImageResize(background, screenWidth, screenHeight);
    Texture2D bgTexture = LoadTextureFromImage(*background);

    Image *playButton = new Image {LoadImage("resources/image/playbutton.png")};
    ImageResize(playButton, 180, 60);
    Texture2D playButtonTexTure = LoadTextureFromImage(*playButton);
     
    Image *playButton2 = new Image {LoadImage("resources/image/playbutton2.png")};
    ImageResize(playButton2, 180, 60);
    Texture2D playButtonTexTure2 = LoadTextureFromImage(*playButton2);
    //-------------------
    //ENDING background:
    Image *background2 = new Image {LoadImage("resources/image/ending.png")};
	ImageResize(background2, screenWidth, screenHeight);
    Texture2D bgTexture2 = LoadTextureFromImage(*background2);
    
    Image *endButton = new Image {LoadImage("resources/image/endbutton.png")};
    ImageResize(endButton, 180, 60);
    Texture2D endButtonTexTure = LoadTextureFromImage(*endButton);

    Image *endButton2 = new Image {LoadImage("resources/image/endbutton2.png")};
    ImageResize(endButton2, 180, 60);
    Texture2D endButtonTexTure2 = LoadTextureFromImage(*endButton2);
    //-------------------

    bool init = false;
   
    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
                UpdateRender(&bgTexture, background);
                
                BeginDrawing();
                    DrawTextureV(bgTexture, {0,0}, WHITE);
                    DrawTexture(playButtonTexTure, screenWidth/2 - playButtonTexTure.width/2, screenHeight/2 - playButtonTexTure.height/2 + 150, WHITE);
                    Rectangle playButton_Rec = {screenWidth/2 - 180/2, screenHeight/2 - 60/2 + 150, 180, 60};

                    if(CheckCollisionPointRec(GetMousePosition(), playButton_Rec)){
                        DrawTexture(playButtonTexTure2, screenWidth/2 - playButtonTexTure2.width/2, screenHeight/2 - playButtonTexTure2.height/2 + 150, WHITE);
                        if(IsMouseButtonPressed(0)){
                            //go to GAMEPLAY
                            gameWon = false;
                            currentScreen = GAMEPLAY;   
                            if (mazeTexture.id != 0) {
                                UnloadRenderTexture(mazeTexture);
                            }
                            
                            mazeTexture = LoadRenderTexture(screenWidth, screenHeight);
                            
                            GenerateMaze(5, 5);
                            AddLoops(5);

                            cellSize = MeasureCellSize();
                            alterVec = MeasureAlterVec(cellSize);      
                        }
                    }
                EndDrawing();

            }break;
            case GAMEPLAY:
            {
                // IMPORTANT BLOCK BELOW  !!!
                if (!init) {
                    lastAutoRotateTime = GetTime();
                    autoRotateInterval = 2.0f;
                    init = true;

                    ClearWindowState(FLAG_WINDOW_RESIZABLE);
                }

				UpdateMobiObj(player);

                WinCheck();
				InputMove();
                Render(alterVec, cellSize, 0.5);

                if (gameWon){
					ResetVal();
                    currentScreen = ENDING;
                    UnloadRenderTexture(mazeTexture);

                    SetWindowState(FLAG_WINDOW_RESIZABLE);
                }
            }break;

            case ENDING:
            {
                UpdateRender(&bgTexture2, background2);

                BeginDrawing();
                    ClearBackground(BLACK);
                    DrawTextureV(bgTexture2, {0,0}, WHITE);
                    DrawTexture(endButtonTexTure, screenWidth/2 - endButtonTexTure.width/2, screenHeight/2 - endButtonTexTure.height/2 + 250, WHITE);
                    Rectangle endButton_Rec = {screenWidth/2 - 180/2, screenHeight/2 - 60/2 + 250, 180, 60};
                    
                    if (CheckCollisionPointRec(GetMousePosition(), endButton_Rec)){
                        DrawTexture(endButtonTexTure2, screenWidth/2 - endButtonTexTure2.width/2, screenHeight/2 - endButtonTexTure2.height/2 + 250, WHITE);  
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
    UnloadTexture(bgTexture2);
    UnloadTexture(playButtonTexTure2);
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
