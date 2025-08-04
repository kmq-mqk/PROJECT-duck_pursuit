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
float rotationDuration = 1.0f;
int rotationType = 0; // 0 = 90 CW, 1 = 90 CCW, 2 = 180
int directionOffset = 0; // Số lần xoay 90 độ CW (modulo 4)



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

void RotateMaze90CounterClockWise() {
    Cell temp[GRID_WIDTH][GRID_HEIGHT];

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int nx = y;
            int ny = GRID_WIDTH - 1 - x;

            temp[nx][ny].visited = maze[x][y].visited;

            // Đổi hướng    
            temp[nx][ny].topWall = maze[x][y].rightWall;
            temp[nx][ny].rightWall = maze[x][y].bottomWall;
            temp[nx][ny].bottomWall = maze[x][y].leftWall;
            temp[nx][ny].leftWall = maze[x][y].topWall;
        }
    }

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            maze[x][y] = temp[x][y];
        }
    }
}

void RotateMaze180() {
    Cell temp[GRID_WIDTH][GRID_HEIGHT];

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int nx = GRID_WIDTH - 1 - x;
            int ny = GRID_HEIGHT - 1 - y;
            temp[nx][ny].visited = maze[x][y].visited;

            temp[nx][ny].topWall = maze[x][y].bottomWall;
            temp[nx][ny].bottomWall = maze[x][y].topWall;
            temp[nx][ny].leftWall = maze[x][y].rightWall;
            temp[nx][ny].rightWall = maze[x][y].leftWall;
        }
    }

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

void RotatePosition90CounterClockwise(Position &pos) {
    int newX = pos.y;
    int newY = GRID_WIDTH - 1 - pos.x;
    pos.x = newX;
    pos.y = newY;
}
void RotatePosition180(Position &pos) {
    int newX = GRID_HEIGHT - 1 - pos.x;
    int newY = GRID_WIDTH - 1 - pos.y;
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
        int textWidth = MeasureText("You win!", 30);
        DrawText("You win!", SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2, 30, DARKGREEN);
        EndDrawing();
        return;
    }
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };

    int dir = -1;
    if (IsKeyPressed(KEY_UP))    dir = 0;
    if (IsKeyPressed(KEY_RIGHT)) dir = 1;
    if (IsKeyPressed(KEY_DOWN))  dir = 2;
    if (IsKeyPressed(KEY_LEFT))  dir = 3;

    if (dir != -1) {
        int rotateDir = (dir + directionOffset)  % 4;
        MovePlayer(dx[rotateDir], dy[rotateDir]);
    }
}

void AddLoops(int loopCount) {
    int loopsAdded = 0;
    while (loopsAdded < loopCount) {
        int x = GetRandomValue(1, GRID_WIDTH - 2);
        int y = GetRandomValue(1, GRID_HEIGHT - 2);

        int dir = GetRandomValue(0, 3);
        int nx = x, ny = y;

        switch (dir) {
            case 0: ny--; break;
            case 1: ny++; break;
            case 2: nx--; break;
            case 3: nx++; break;
        }

        if (nx >= 0 && ny >= 0 && nx < GRID_WIDTH && ny < GRID_HEIGHT) {
            // Kiểm tra xem có tường chưa bị phá
            if (nx == x && ny == y - 1 && maze[x][y].topWall && maze[nx][ny].bottomWall) {
                maze[x][y].topWall = false;
                maze[nx][ny].bottomWall = false;
                loopsAdded++;
            } else if (nx == x && ny == y + 1 && maze[x][y].bottomWall && maze[nx][ny].topWall) {
                maze[x][y].bottomWall = false;
                maze[nx][ny].topWall = false;
                loopsAdded++;
            } else if (nx == x - 1 && ny == y && maze[x][y].leftWall && maze[nx][ny].rightWall) {
                maze[x][y].leftWall = false;
                maze[nx][ny].rightWall = false;
                loopsAdded++;
            } else if (nx == x + 1 && ny == y && maze[x][y].rightWall && maze[nx][ny].leftWall) {
                maze[x][y].rightWall = false;
                maze[nx][ny].leftWall = false;
                loopsAdded++;
            }
        }
    }
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Penguin Test");
    mazeTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    InitAudioDevice();
    InitializeMaze();
    GenerateMaze(0, 0);
    AddLoops(5);
    player.x = 0, player.y = 0;
    goal.x = GRID_WIDTH - 1;
    goal.y = GRID_HEIGHT - 1;
    double lastAutoRotateTime = GetTime();
    float autoRotateInterval = 5.0f;

    while(!WindowShouldClose()) {
        if (!isRotating && GetTime() - lastAutoRotateTime >= autoRotateInterval) {
            isRotating = true;
            rotationStartTime = GetTime();
            lastAutoRotateTime = GetTime();
        
            rotationType = GetRandomValue(0, 2); // 0=CW, 1=CCW, 2=180
        
            switch (rotationType) {
                case 0: targetAngle = 90.0f; break;
                case 1: targetAngle = -90.0f; break;
                case 2: targetAngle = 180.0f; break;
            }
        }
        
        
        
        inputMove();
        if (isRotating) {
            float t = (GetTime() - rotationStartTime) / rotationDuration;
            if (t >= 1.0f) {
                isRotating = false;
                rotationAngle = 0.0f;
            
                // Xoay dữ liệu thật
                switch (rotationType) {
                    case 0: // 90 CW
                        directionOffset = (directionOffset + 1) % 4;
                        RotateMaze90Clockwise();
                        RotatePosition90Clockwise(player);
                        RotatePosition90Clockwise(goal);
                        break;
                    case 1: // 90 CCW
                        directionOffset = (directionOffset + 3) % 4;
                        RotateMaze90CounterClockWise();
                        RotatePosition90CounterClockwise(player);
                        RotatePosition90CounterClockwise(goal);
                        break;
                    case 2: // 180
                        directionOffset = (directionOffset + 2) % 4;
                        RotateMaze180();
                        RotatePosition180(player);
                        RotatePosition180(goal);
                        break;
                }
                
            }
            else {
                rotationAngle = t * targetAngle;
            }
            
        }
        
        if (gameWon) continue;
    // Vẽ maze lên texture
    BeginTextureMode(mazeTexture);
        ClearBackground(BLACK);
        DrawMaze();
        DrawPlayer();
        DrawGoal();
    EndTextureMode();

    // Sau đó xoay texture lên màn hình
    BeginDrawing();
    ClearBackground(BLACK);

    Rectangle source = { 0, 0, (float)SCREEN_WIDTH, -(float)SCREEN_HEIGHT }; // cần - chiều cao
    Rectangle dest = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    DrawTexturePro(mazeTexture.texture, source, dest, origin, rotationAngle, WHITE);

    if (gameWon){
        int textWidth = MeasureText("You win!", 30);
        DrawText("You win!", SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2, 30, DARKGREEN);
    }
        
    EndDrawing();

    }
    UnloadRenderTexture(mazeTexture);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}