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
#include <stdio.h>

//extern bool gameWon;
//extern RenderTexture2D mazeTexture;
//extern double lastAutoRotateTime;
//extern float autoRotateInterval;
//
extern Vector2 initScreenSize;
//extern MobiObj player;
//extern Vector2 alterVec;
//extern double cellSize;


void GameStart(){
    bool init = false;
	bool gameWon = false;
	int screenWidth = (int)initScreenSize.x;
	int screenHeight = (int)initScreenSize.y;
	RenderTexture lastTexture = LoadRenderTexture(screenWidth, screenHeight);
	Obj* mobi;
	Obj* rota;

	RenderList list;

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
//						gameWon = false;
						currentScreen = GAMEPLAY;   
						if (lastTexture.id != 0) {
						    UnloadRenderTexture(lastTexture);
						}
						
						lastTexture = LoadRenderTexture(screenWidth, screenHeight);

						MazeLoadingArgs args;
						args.type = GENERATE;
						args.data.mazeSize = (Vector2){5, 7};
						double cell = MeasureCellSize((Vector2){screenWidth, screenHeight}, (Vector2){5, 7});
						rota = New_RotateObj(args, 2.0, 0.5, (int)cell, (int)cell);

						mobi = New_MobiObj("assets/image/sprite/get_a_job.jpeg", 0.5, (int)cell, (int)cell);

						MobiObj* mobiList[] = {(MobiObj*)mobi};
						size_t mobiCount = sizeof(mobiList) / sizeof(mobiList[0]);
						RotateObj* rotaList[] = {(RotateObj*)rota};
						size_t rotaCount = sizeof(rotaList) / sizeof(rotaList[0]);
						list = (RenderList){mobiCount, mobiList, rotaCount, rotaList};

							printf("%u _ %u\n", mobiCount, rotaCount);
							RotateObj* test = rotaList[0];
							printf("%.2f\t%.2f\n", test->_dir, test->_speed);
							printf("%.2f\t%.2f\n", test->_lastRotateTime, test->_rotateInterval);
							printf("%.2f\t%.2f\n", test->_curAngle, test->_tarAngle);
//							exit(EXIT_SUCCESS);


//						GenerateMaze(5, 5);
//						AddLoops(5);
						
//						cellSize = MeasureCellSize();
//						alterVec = MeasureAlterVec(cellSize);      
					}
				}
                EndDrawing();

            }break;
            case GAMEPLAY:
            {
				screenWidth = GetScreenWidth();
				screenHeight = GetScreenHeight();
				Vector2 screenSize = {(float)screenWidth, (float)screenHeight};
				Maze* mazeInfo = ((RotateObj*)rota)->GetMazeInfo((RotateObj*)rota);
				Vector2 mazeSize = {(float)mazeInfo->col, (float)mazeInfo->row};

				double cell = MeasureCellSize(screenSize, mazeSize);
				Vector2 alterV = MeasureAlterVec(screenSize, mazeSize, cell); 

                // IMPORTANT BLOCK BELOW  !!!
//				if (!init) {
//					lastAutoRotateTime = GetTime();
//					autoRotateInterval = 2.0f;
//					init = true;
//					
////					ClearWindowState(FLAG_WINDOW_RESIZABLE);
//				}

				UpdateRender(&lastTexture, (MobiObj*)mobi, (RotateObj*)rota, screenWidth, screenHeight);

//                WinCheck();
				InputMove((MobiObj*)mobi, mazeInfo);

//				Render(list, &lastTexture, alterV, cell);
				Render(list, &lastTexture, (Vector2) {0, 0}, 30);

//                Render(alterVec, cellSize, 0.5);

				Vector2 mobiPos = mobi->GetPos(mobi);
				Vector2 goalPos = {mazeSize.x - 1, mazeSize.y - 1};
				if (WinCheck(mobiPos, goalPos)) {
//					ResetVal();
					currentScreen = ENDING;
					UnloadRenderTexture(lastTexture);
					
//					SetWindowState(FLAG_WINDOW_RESIZABLE);
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
