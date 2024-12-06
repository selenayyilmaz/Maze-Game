#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WALL 1
#define EMPTY 0
#define START 'S'
#define END 'E'
#define PLAYER 'P'
#define VISITED 2


const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

bool is_within_bounds(int x, int y, int size) {
    return x >= 0 && y >= 0 && x < size && y < size;
}

bool flood_fill(int **maze, int x, int y, int size) {
    if (!is_within_bounds(x, y, size) || maze[x][y] != EMPTY) {
        return false;
    }

    if (x == size - 1 && y == size - 1) {
        return true;
    }

    maze[x][y] = VISITED;

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (flood_fill(maze, nx, ny, size)) {
            return true;
        }
    }

    return false;
}


void create_labyrinth_with_path(int **maze, int size, int wall_probability) {
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            maze[i][j] = (rand() % 15 == 0) ? EMPTY : WALL;
        }
    }

    maze[0][0] = EMPTY;
    maze[size - 1][size - 1] = EMPTY;

    while (!flood_fill(maze, 0, 0, size)) {
        int rx = rand() % size;
        int ry = rand() % size;
        maze[rx][ry] = EMPTY;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (maze[i][j] == VISITED) {
                maze[i][j] = EMPTY;
            }
        }
    }

    maze[0][0] = START;
    maze[size - 1][size - 1] = END;
}

void print_labyrinth(int **maze, int size, int px, int py) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == px && j == py) {
                printf("%c ", PLAYER);
            } else if (maze[i][j] == WALL) {
                printf("# ");
            } else if (maze[i][j] == EMPTY) {
                printf(". ");
            } else if (maze[i][j] == START) {
                printf("%c ", START);
            } else if (maze[i][j] == END) {
                printf("%c ", END);
            }
        }
        printf("\n");
    }
}

void start_game(int **maze, int size) {
    int x = 0, y = 0;
    int move_count = 0;
    char move;

    printf("Oyun basladi. Amaciniz bitis noktasina ulasmak. Basarilar!\n");

    while (x != size - 1 || y != size - 1) {
        print_labyrinth(maze, size,x,y);

        printf("W-A-S-D ile hareket edin: ");
        scanf(" %c", &move);

        int new_x = x, new_y = y;

        switch (move) {
            case 'W': case 'w': new_x--; break;
            case 'S': case 's': new_x++; break;
            case 'A': case 'a': new_y--; break;
            case 'D': case 'd': new_y++; break;
            default:
                printf("Gecersiz tus! WASD tuslarini kullanin.\n");
                continue;
        }

        if (is_within_bounds(new_x, new_y, size) && maze[new_x][new_y] == EMPTY || maze[new_x][new_y] == END) {
            x = new_x;
            y = new_y;
            move_count++;
        } else {
            printf("Duvardan gecemezsiniz!\n");
        }
    }

    printf("Tebrikler! Labirentten cikmayi basardiniz.\n");
    printf("Cikmak icin %d hamle yaptiniz.\n",move_count);
}

int main() {
    int size;
    int size_option;

    printf("Boyut seciniz. 1, 2 ya da 3 olarak belirtiniz.\n");
    printf("1 --> 10x10\n");
    printf("2 --> 15x15\n");
    printf("3 --> 20x20\n");
    scanf("%d", &size_option);

    switch (size_option) {
        case 1: size = 10; break;
        case 2: size = 15; break;
        case 3: size = 20; break;
        default:
            printf("Yanlis secim.\n");
            size = 10;
            break;
    }

    int **maze = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        maze[i] = (int *)malloc(size * sizeof(int));
    }

    create_labyrinth_with_path(maze, size,90);
    start_game(maze, size);

    for (int i = 0; i < size; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}