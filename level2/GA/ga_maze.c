#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROWS 8
#define COLS 10

static const char *MAZE[ROWS] = {
    "HHHHHHHHHH",
    "o  HH     ",
    "H HH HH HH",
    "H  HH H HH",
    "H  H H  HH",
    "H H HH HHH",
    "H        H",
    "HHHHHHHHHH"
};

static const int START_X = 1, START_Y = 0;
static const int END_X = 1, END_Y = 9;

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

#define CHROMO_LEN 20
#define POP_SIZE 200
#define GENERATIONS 200
#define MUTATE_RATE 0.1

struct Chromo {
    int moves[CHROMO_LEN];
    int fitness;
};

int apply_moves(const int moves[], int path[][2]) {
    int x = START_X, y = START_Y;
    int len = 0;
    path[len][0] = x; path[len][1] = y; len++;
    for (int i = 0; i < CHROMO_LEN; ++i) {
        int m = moves[i];
        int nx = x, ny = y;
        if (m == MOVE_UP) nx--; else if (m == MOVE_DOWN) nx++; else if (m == MOVE_LEFT) ny--; else ny++;
        if (nx < 0 || nx >= ROWS || ny < 0 || ny >= COLS) continue;
        if (MAZE[nx][ny] != 'H') { x = nx; y = ny; path[len][0] = x; path[len][1] = y; len++; }
        if (x == END_X && y == END_Y) break;
    }
    return len;
}

int fitness(const int moves[]) {
    int path[CHROMO_LEN+1][2];
    int len = apply_moves(moves, path);
    int x = path[len-1][0];
    int y = path[len-1][1];
    int dist = abs(x - END_X) + abs(y - END_Y);
    return -dist;
}

void random_chromo(int moves[]) {
    for (int i = 0; i < CHROMO_LEN; ++i) moves[i] = rand() % 4;
}

void copy_moves(int dest[], const int src[]) { for (int i=0;i<CHROMO_LEN;++i) dest[i]=src[i]; }

void mutate(int moves[]) {
    for (int i=0;i<CHROMO_LEN;++i) if ((double)rand()/RAND_MAX < MUTATE_RATE) moves[i] = rand()%4; }

void crossover(const int a[], const int b[], int child[]) {
    int point = rand() % (CHROMO_LEN-2) + 1;
    for (int i=0;i<point;++i) child[i]=a[i];
    for (int i=point;i<CHROMO_LEN;++i) child[i]=b[i];
}

void draw_path(const int path[][2], int len) {
    char maze[ROWS][COLS+1];
    for (int i=0;i<ROWS;++i) strncpy(maze[i], MAZE[i], COLS+1);
    for (int i=0;i<len;++i) {
        int x=path[i][0], y=path[i][1];
        if (maze[x][y]==' ' || maze[x][y]=='o') maze[x][y] = '.';
    }
    for (int i=0;i<ROWS;++i) {
        printf("%.*s\n", COLS, maze[i]);
    }
}

int main() {
    srand(0);
    struct Chromo pop[POP_SIZE];
    for (int i=0;i<POP_SIZE;++i) { random_chromo(pop[i].moves); pop[i].fitness = fitness(pop[i].moves); }
    int best_moves[CHROMO_LEN];
    int best_path[CHROMO_LEN+1][2];
    int best_len=0, best_fit=-1000;
    clock_t start = clock();
    for (int g=0; g<GENERATIONS; ++g) {
        struct Chromo new_pop[POP_SIZE];
        for (int i=0;i<POP_SIZE;++i) {
            // tournament selection
            int besta = rand()%POP_SIZE; for(int k=1;k<5;++k){ int j=rand()%POP_SIZE; if(pop[j].fitness>pop[besta].fitness) besta=j; }
            int bestb = rand()%POP_SIZE; for(int k=1;k<5;++k){ int j=rand()%POP_SIZE; if(pop[j].fitness>pop[bestb].fitness) bestb=j; }
            int child[CHROMO_LEN];
            crossover(pop[besta].moves, pop[bestb].moves, child);
            mutate(child);
            copy_moves(new_pop[i].moves, child);
            new_pop[i].fitness = fitness(child);
            if (new_pop[i].fitness > best_fit) {
                best_fit = new_pop[i].fitness;
                copy_moves(best_moves, child);
                best_len = apply_moves(child, best_path);
                if (best_path[best_len-1][0]==END_X && best_path[best_len-1][1]==END_Y) {
                    clock_t end = clock();
                    draw_path(best_path, best_len);
                    printf("time: %.3fs\n", (double)(end-start)/CLOCKS_PER_SEC);
                    return 0;
                }
            }
        }
        memcpy(pop, new_pop, sizeof(pop));
    }
    clock_t end = clock();
    best_len = apply_moves(best_moves, best_path);
    draw_path(best_path, best_len);
    printf("time: %.3fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    return 0;
}

