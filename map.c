#include "map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ALL NECESSARY GLOBAL VARIABLES FOR MAP COME FROM HERE
Cell** maze;    // maze[col][row]
int col;    // Ox
int row;    // Oy
Position player, goal;

void GenerateMaze(int inputCol, int inputRow) {
    col = inputCol;
    row = inputRow;
    maze = (Cell**)calloc(row, sizeof(Cell*));
    for (int j = 0; j < row; j++) {
        maze[j] = (Cell*)calloc(col,  sizeof(Cell));
    }

    player = (Position) {0, 0};
    goal = (Position) {col - 1, row - 1};

    InitializeMaze();
    CreateMaze(0, 0);
}
void InitializeMaze() {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            maze[i][j].visited = false;
            maze[i][j].topWall = true;
            maze[i][j].bottomWall = true;
            maze[i][j].leftWall = true;
            maze[i][j].rightWall = true;
        }
    }
}
void CreateMaze(int x, int y) {
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
            
            if (nx >= 0 && nx < col && ny >= 0 && ny < row && !maze[nx][ny].visited) {
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
                CreateMaze(nx, ny);
                moved = true;
                break;
            }
        }
        if (!moved) break; // Try 4 directions but cant => break while
    }
}
void AddLoops(int loopCount) {
    int loopsAdded = 0;
    while (loopsAdded < loopCount) {
        int x = GetRandomValue(1, col - 2);
        int y = GetRandomValue(1, row - 2);

        int dir = GetRandomValue(0, 3);
        int nx = x, ny = y;

        switch (dir) {
            case 0: ny--; break;
            case 1: ny++; break;
            case 2: nx--; break;
            case 3: nx++; break;
        }

        if (nx >= 0 && ny >= 0 && nx < col && ny < row) {
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



void ReadTxt(int j, char* line, int n) {
	int i = 0;
	for (int k = 0; k < n; k++) {
		char ch = line[k];
        // printf("%c_", line[k]);

		switch (ch) {
			case '|':
				i++;
				// continue;
				break;
			case 'a':
                player.x = i;
                player.y = j;
				break;
			case 'b':
                goal.x = i;
                goal.y = j;
				break;
			case 'l':
				maze[i][j].leftWall = true;
				break;
			case 'r':
				maze[i][j].rightWall = true;
				break;
			case 'u':
                maze[i][j].topWall = true;
				break;
			case 'd':
				maze[i][j].bottomWall = true;
				break;
			default:
				fprintf(stderr, "Your .txt map is so ass, unknown character appears\n");
				exit(1);
				break;
		}
	}
}
void LoadMap(char* fileName) {
	FILE* fin = fopen(fileName, "r");
	if (!fin) {
		perror("Error opening map file");
		exit(1);
	}
	fscanf(fin, "%d %d ", &row, &col);

    // INIT MAZE
	maze = (Cell**)calloc(col, sizeof(Cell*));
	for (int i = 0; i < col; i++)
		maze[i] = (Cell*)calloc(row, sizeof(Cell));

    // READ FILE
    for (int j = 0; j < row; j++) {
		char* line = NULL;
		size_t len;
		int n = getline(&line, &len, fin);
		n -= (line[n - 1] == '\n');
		ReadTxt(j, line, n);
	}

	fclose(fin);
}
void WriteMap(char* fileName) {
    int len = strlen(fileName), i = 0;
    char* logFileName = (char*)calloc(len + strlen(".log") + 1, sizeof(char));
	logFileName[i++] = '.';
    for (i; i <= len; i++)
        logFileName[i] = fileName[i - 1];
    sprintf(logFileName, "%s.log", logFileName);


    FILE* fmap = fopen(fileName, "w");
    FILE* flog = fopen(logFileName, "w");
    free(logFileName);

    fprintf(fmap, "%d %d\n", col, row);
    fprintf(flog, "%d %d\n", col, row);

    for (int j = 0; j < row; j++) {
        for (int i = 0; i < col; i++) {
            fprintf(flog, "maze[%d][%d] = \t", i, j);
            if (player.x == i && player.y == j) {
                fprintf(flog, "SRC-");
                fprintf(fmap, "a");
            }
            if (goal.x == i && goal.y == j) {
                fprintf(flog, "DEST-");
                fprintf(fmap, "b");
            }
            if (maze[i][j].topWall == true) {
                fprintf(flog, "top-");
                fprintf(fmap, "u");
            }
            if (maze[i][j].bottomWall == true) {
                fprintf(flog, "bot-");
                fprintf(fmap, "d");
            }
            if (maze[i][j].leftWall == true) {
                fprintf(flog, "left-");
                fprintf(fmap, "l");
            }
            if (maze[i][j].rightWall == true) {
                fprintf(flog, "right-");          
                fprintf(fmap, "r");
            }
            fprintf(flog, "\n");
            fprintf(fmap, "|");
        }
    }

    fclose(fmap);
    fclose(flog);
}
