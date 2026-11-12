#ifndef PLAYER_H
#define PLAYER_H
#define INVENTORY_SIZE 10

struct Player {
	char name[32];
	int hp, max_hp;
	int stamina, max_stamina;
	int gold;
	int x, y;
};

#endif 