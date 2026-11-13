#ifndef MAP_H
#define MAP_H
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30
#define WALL '#'
#define EMPTY '.'
#define HERO '@'
#define TREE 'T'
#define ROCK 'R'
#define PORTAL 'P'
#include "player.h"


struct Map {
	char** map;
	int width;
	int height;
	int floor;
};
void generate_map(struct Map* m, struct Player* p);
void print_map(struct Map* m);
void free_map(struct Map* m);
#endif
