#ifndef UTILS_H
#define UTILS_H
#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define HOURS_IN_DAY 24
#define START_HOUR 8
#include "player.h"

struct World {
	int day;
	int hour;
};
int durak(char message[]);
void show_time(struct World* w);
void change_time(struct World* w);
int check_player(struct Player* p);
void load_items(char item_names[][MAX_NAME_LEN]);
void process_logs();
#endif