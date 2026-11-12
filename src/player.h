#ifndef PLAYER_H
#define PLAYER_H
#define INVENTORY_SIZE 10
#define MAX_NAME_LEN 32

struct Player {
	char name[32];
	int hp, max_hp;
	int stamina, max_stamina;
	int gold;
	int x, y;
	int inventory[INVENTORY_SIZE];
};
void create_player(struct Player** p);
void show_player_card(struct Player *p);
void show_inventory(struct Player* p, char item_names[][MAX_NAME_LEN]);
void put_item(struct Player* p, char item_names[][MAX_NAME_LEN]);
void delete_item(struct Player* p, char item_names[][MAX_NAME_LEN]);
#endif 