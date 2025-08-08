#include "header.hpp"

Penguin::Penguin(){


}

Penguin::~Penguin() {

}

void Penguin::Draw() {

}

void Penguin::Update() {
    
}

#include "map.hpp"
#include "render.hpp"

extern bool gameWon;
extern RenderTexture2D mazeTexture;
extern double lastAutoRotateTime;
extern float autoRotateInterval;

void GameStart(){
    GameScreen currentScreen = OPENING;
    InitAudioDevice();

    bool init = false;
   
    while(!WindowShouldClose()) {
        BeginDrawing();
        switch (currentScreen){
            case OPENING:
            {
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
                    
                    GenerateMaze(5, 5);
                    AddLoops(5);
                }
            }break;
            case GAMEPLAY:
            {
                ClearBackground(BLACK);
                // 3 LINES BELOW ARE IMPORTANT  !!!
                if (!init) {
                    lastAutoRotateTime = GetTime();
                    autoRotateInterval = 5.0f;
                    init = true;
                }
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
                init = false;
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