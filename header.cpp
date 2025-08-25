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

//extern bool gameWon;
//extern RenderTexture2D mazeTexture;
//extern double lastAutoRotateTime;
//extern float autoRotateInterval;
//
extern Vector2 screenSize;
//extern MobiObj player;
//extern Vector2 alterVec;
//extern double cellSize;


void GameStart(){
    bool init = false;
	bool gameWon = false;
	int screenWidth = (int)screenSize.x;
	int screenHeight = (int)screenSize.y;
	RenderTexture lastTexture = LoadRenderTexture(screenWidth, screenHeight);
	MobiObj* mobi;
	RotateObj* rota;

    GameScreen currentScreen = OPENING;
    InitAudioDevice();

    // OPENING background:
    Image *background = new Image {LoadImage("assets/image/background/background.png")};
	ImageResize(background, screenWidth, screenHeight);
    Texture2D bgTexture = LoadTextureFromImage(*background);

    Image *playButton = new Image {LoadImage("assets/image/button/playbutton.png")};
    ImageResize(playButton, 180, 60);
    Texture2D playButtonTexTure = LoadTextureFromImage(*playButton);
     
    Image *playButton2 = new Image {LoadImage("assets/image/button/playbutton2.png")};
    ImageResize(playButton2, 180, 60);
    Texture2D playButtonTexTure2 = LoadTextureFromImage(*playButton2);
    //-------------------
    //ENDING background:
    Image *background2 = new Image {LoadImage("assets/image/background/ending.png")};
	ImageResize(background2, screenWidth, screenHeight);
    Texture2D bgTexture2 = LoadTextureFromImage(*background2);
    
    Image *endButton = new Image {LoadImage("assets/image/button/endbutton.png")};
    ImageResize(endButton, 180, 60);
    Texture2D endButtonTexTure = LoadTextureFromImage(*endButton);

    Image *endButton2 = new Image {LoadImage("assets/image/button/endbutton2.png")};
    ImageResize(endButton2, 180, 60);
    Texture2D endButtonTexTure2 = LoadTextureFromImage(*endButton2);
    //-------------------
   

    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
				screenWidth = GetScreenWidth();
				screenHeight = GetScreenHeight();

                UpdateTextureFromImage(&bgTexture, background, screenWidth, screenHeight);
                
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
                            if (lastTexture.id != 0) {
                                UnloadRenderTexture(lastTexture);
                            }
                            
                            lastTexture = LoadRenderTexture(screenWidth, screenHeight);
                            
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
				screenWidth = GetScreenWidth();
				screenHeight = GetScreenHeight();

                // IMPORTANT BLOCK BELOW  !!!
				if (!init) {
					lastAutoRotateTime = GetTime();
					autoRotateInterval = 2.0f;
					init = true;
					
//					ClearWindowState(FLAG_WINDOW_RESIZABLE);
				}

				UpdateRender(&lastTexture, mobi, rota, screenWidth, screenHeight);

                WinCheck();
				InputMove();
                Render(alterVec, cellSize, 0.5);

				if (gameWon){
					ResetVal();
					currentScreen = ENDING;
					UnloadRenderTexture(lastTexture);
					
					SetWindowState(FLAG_WINDOW_RESIZABLE);
				}
            }break;

            case ENDING:
            {
				screenWidth = GetScreenWidth();
				screenHeight = GetScreenHeight();

                UpdateTextureFromImage(&bgTexture2, background2, screenWidth, screenHeight);

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
