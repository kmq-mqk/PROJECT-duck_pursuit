#pragma once
#include "header.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

Cell maze[GRID_WIDTH][GRID_HEIGHT];
Position player, goal;
bool gameWon = false;

RenderTexture2D mazeTexture;
float rotationAngle = 0.0f;
float targetAngle = 0.0f;
bool isRotating = false;
double rotationStartTime = 0.0;
float rotationDuration = 0.5f;


void InitializeMaze() {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            maze[i][j].visited = false;
            maze[i][j].topWall = true;
            maze[i][j].bottomWall = true;
            maze[i][j].leftWall = true;
            maze[i][j].rightWall = true;
        }
    }
}

void GenerateMaze(int x, int y) {
    maze[x][y].visited = true;
    while (true) {
        int directions[] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++) {
            int j = GetRandomValue(i, 3);
            int temp = directions[i];
            directions[i] = directions[j];
            directions[j] = temp;
        }

        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int nx = x, ny = y;
            if (directions[i] == 0) ny -= 1; // Up
            else if (directions[i] == 1) nx += 1; // Right
            else if (directions[i] == 2) ny += 1; // Down
            else if (directions[i] == 3) nx -= 1; // Left
            
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT && !maze[nx][ny].visited) {
                if (directions[i] == 0) {
                    maze[x][y].topWall = false;
                    maze[nx][ny].bottomWall = false;
                }
                else if (directions[i] == 1) {
                    maze[x][y].rightWall = false;
                    maze[nx][ny].leftWall = false;
                }
                else if (directions[i] == 2) {
                    maze[x][y].bottomWall = false;
                    maze[nx][ny].topWall = false;
                }
                else if (directions[i] == 3) {
                    maze[x][y].leftWall = false;
                    maze[nx][ny].rightWall = false;
                }
                GenerateMaze(nx, ny);
                moved = true;
                break;
            }
        }
        if (!moved) break; // Try 4 directions but cant => break while
    }
}

void DrawMaze() {
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int posX = x * CELL_SIZE;
            int posY = y * CELL_SIZE;
            if (maze[x][y].topWall)  DrawLine(posX, posY, posX + CELL_SIZE, posY, WHITE);
            if (maze[x][y].rightWall)  DrawLine(posX + CELL_SIZE, posY, posX + CELL_SIZE, posY + CELL_SIZE, WHITE);
            if (maze[x][y].leftWall)  DrawLine(posX, posY , posX, posY + CELL_SIZE, WHITE);
            if (maze[x][y].bottomWall)  DrawLine(posX, posY + CELL_SIZE, posX + CELL_SIZE, posY + CELL_SIZE, WHITE);

        }
    }
}

void RotateMaze90Clockwise() {
    Cell temp[GRID_WIDTH][GRID_HEIGHT];

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int nx = GRID_HEIGHT - 1 - y;
            int ny = x;

            temp[nx][ny].visited = maze[x][y].visited;

            // đổi hướng
            temp[nx][ny].topWall = maze[x][y].leftWall;
            temp[nx][ny].rightWall = maze[x][y].topWall;
            temp[nx][ny].bottomWall = maze[x][y].rightWall;
            temp[nx][ny].leftWall = maze[x][y].bottomWall;
        }
    }

    // sao chép lại vào maze gốc
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            maze[x][y] = temp[x][y];
        }
    }
}

void RotatePosition90Clockwise(Position &pos) {
    int newX = GRID_HEIGHT - 1 - pos.y;
    int newY = pos.x;
    pos.x = newX;
    pos.y = newY;
}


void DrawPlayer() {
    DrawRectangle(player.x * CELL_SIZE + 2, player.y * CELL_SIZE + 2, CELL_SIZE - 4, CELL_SIZE - 4, GREEN);
}

void DrawGoal() {
    DrawRectangle(goal.x * CELL_SIZE + 2, goal.y * CELL_SIZE + 2, CELL_SIZE - 4, CELL_SIZE - 4, PINK);
}

void MovePlayer(int dx, int dy) {
    int newX = player.x + dx;
    int newY = player.y + dy;
    if (newX < 0 || newX >= GRID_WIDTH || newY < 0 || newY >= GRID_HEIGHT) return;
    if (dx == -1 && maze[player.x][player.y].leftWall) return;
    if (dx == 1 && maze[player.x][player.y].rightWall) return;
    if (dy == -1 && maze[player.x][player.y].topWall) return;
    if (dy == 1 && maze[player.x][player.y].bottomWall) return;

    player.x = newX;
    player.y = newY;

    if (player.x == goal.x && player.y == goal.y) gameWon = true;
}

void inputMove() {
    if (gameWon) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("You win! ", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 30, DARKGREEN);
        EndDrawing();
    }
    if (IsKeyPressed(KEY_RIGHT)) MovePlayer(1, 0);
    if (IsKeyPressed(KEY_LEFT)) MovePlayer(-1, 0);
    if (IsKeyPressed(KEY_UP)) MovePlayer(0, -1);
    if (IsKeyPressed(KEY_DOWN)) MovePlayer(0, 1);
}

void GameStart(){
    GameScreen currentScreen = OPENING;

    SetTargetFPS(60);
    InitAudioDevice();
   

    while(!WindowShouldClose()) {
        switch (currentScreen){
            case OPENING:
            {
                Rectangle playButton = {(SCREEN_WIDTH - MeasureText("PLAY", 100)) / 2, 500, MeasureText("PLAY", 100), 40};

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
                if (!isRotating && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isRotating = true;
                    rotationStartTime = GetTime();
                    targetAngle = 90.0f;
                }
                inputMove();
                if (isRotating) {
                    float t = (GetTime() - rotationStartTime) / rotationDuration;
                    if (t >= 1.0f) {
                        isRotating = false;
                        rotationAngle = 0.0f;
                
                        // Lúc này mới xoay thật
                        RotateMaze90Clockwise();
                        RotatePosition90Clockwise(player);
                        RotatePosition90Clockwise(goal);
                    } else {
                        rotationAngle = t * targetAngle; // t : 0 -> 1
                    }
                }
                if (gameWon){
                    currentScreen = ENDING;
                    UnloadRenderTexture(mazeTexture);
                }
            }break;

            case ENDING:
            {
                Rectangle playAgainButton = {(SCREEN_WIDTH - MeasureText("PLAY AGAIN", 100)) / 2, 500, MeasureText("PLAY AGAIN", 100), 40};
                if (GuiButton(playAgainButton, "PLAY AGAIN"))
                currentScreen = OPENING;
            }break;
            default: break;
        }


        // Vẽ giao diện
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch(currentScreen)
        {
            case OPENING:
            {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SKYBLUE);
                DrawText("DUCK PURSUIT", 100, 300, 80, ORANGE);
                Rectangle playButton = {(SCREEN_WIDTH - MeasureText("PLAY", 100)) / 2, 500, MeasureText("PLAY", 100), 40};
                int button1 = GuiButton(playButton, "PLAY");
            }break;

            case GAMEPLAY:
            {
                // Vẽ maze lên texture
                BeginTextureMode(mazeTexture);
                ClearBackground(BLACK);
                DrawMaze();
                DrawPlayer();
                DrawGoal();
                EndTextureMode();

                // Sau đó xoay texture lên màn hình
                Rectangle source = { 0, 0, (float)SCREEN_WIDTH, -(float)SCREEN_HEIGHT }; // cần - chiều cao
                Rectangle dest = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
                Vector2 origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

                DrawTexturePro(mazeTexture.texture, source, dest, origin, rotationAngle, WHITE);
            }break;

            case ENDING:
            {
                DrawText("You win!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 , 30, DARKGREEN);
                Rectangle playAgainButton = {(SCREEN_WIDTH - MeasureText("PLAY AGAIN", 100)) / 2, 500, MeasureText("PLAY AGAIN", 100), 40};
                int button2 = GuiButton(playAgainButton, "PLAY AGAIN");
            }break;
            default: break;
        }

    
        EndDrawing();

    }

    CloseAudioDevice();
}
