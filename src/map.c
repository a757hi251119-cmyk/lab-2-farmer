#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "map.h"
#include "utils.h"


void generate_map(struct Map *m, struct Player *p) {
    m->height = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
    m->width = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
    m->map = (char**)malloc(m->height * sizeof(char*));
    for (int i = 0; i < m->height; i++) {
        m->map[i] = (char*)malloc(m->width * sizeof(char));
    }
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (i > 0 && i < m->height - 1 && j > 0 && j < m->width - 1) {
                if (rand() % 100 < 20) {
                    if (rand() % 2 == 0) {
                        m->map[i][j] = ROCK;
                    }
                    else {
                        m->map[i][j] = TREE;
                    }
                }
                else {
                    m->map[i][j] = EMPTY;
                }
            }
            else {
                m->map[i][j] = EMPTY;
            }
        }
    }
    while (true) {
        p->x = rand() % m->width;
        p->y = rand() % m->height;
        if (m->map[p->y][p->x] == EMPTY) {
            m->map[p->y][p->x] = HERO;
            break;
        }
    }
}

void print_map(struct Map *m) {
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            printf("%c", m->map[i][j]);
        }
        printf("\n");
    }
}
