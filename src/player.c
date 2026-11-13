#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "utils.h"

void create_player(struct Player** p) {
    if (*p != NULL) {
        printf("Сначала создайте персонажа!\n");
        int ans;
        printf("Хотите пересоздать персонажа? 0 - нет, 1 - да\n");
        scanf("%d", &ans);
        if (ans == 0) {
            return;
        }
        else {
            free(*p);
            *p = NULL;
        }
    }
    *p = (struct Player*)malloc(sizeof(struct Player));
    if (*p == NULL) { 
        printf("Ошибка памяти\n"); 
        return; 
    }
    while (getchar() != '\n');
    printf("Введите имя: ");
    fgets((*p)->name, 32, stdin);
    (*p)->name[strcspn((*p)->name, "\n")] = '\0';
    (*p)->hp = 100;
    (*p)->max_hp = 100;
    (*p)->stamina = 50;
    (*p)->max_stamina = 50;
    (*p)->gold = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        (*p)->inventory[i] = 0;
    }
}

void show_player_card(struct Player* p) {
    if (!check_player(p)) return;
    printf("=== Карточка персонажа ===\n");
    printf("Имя: %s\n", p->name);
    printf("HP: %d/%d\n", p->hp, p->max_hp);
    printf("Stamina: %d/%d\n", p->stamina, p->max_stamina);
    printf("Золото: %d\n", p->gold);
    printf("Инвентарь:\n");

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] != 0) {
            printf("Слот %d: %d\n", i, p->inventory[i]);
        }
    }
}
void show_inventory(struct Player *p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    printf("\n===Инвентарь===\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] - %s\n", i, p->inventory[i], item_names[p->inventory[i]]);
    }
}

void put_item(struct Player *p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    int slot = durak("Куда положить?\n");
    if (slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Нет такого слота\n");
        return;
    }
    int obj = durak("Что положить?\n");
    p->inventory[slot] = obj;
    printf("Добавлено %s", item_names[p->inventory[slot]]);
}
void delete_item(struct Player *p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    int slot = durak("Какой слот очистить?\n");
    if (slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Нет такого слота\n");
        return;
    }
    p->inventory[slot] = 0;
    printf("Слот очищен\n");
}
void delate_all_items(struct Player* p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    int obj = durak("От какого предмета очистить инвентарь?\n");
    int count_slot = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] == obj) {
            p->inventory[i] = 0;
            count_slot++;
        }
    }
    printf("Было очищено %d слотов от %s", count_slot, item_names[obj]);
}
void find_item(struct Player *p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    char search_name[32];
    printf("Какой предмет хотите найти?\n");
    scanf("%31s", search_name);
    int found_id = -1;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (strcmp(item_names[i], search_name) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Предмет не найден в каталоге\n");
        return;
    }

    int found_in_inv = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] == found_id) {
            printf("Найден в слоте %d\n", i);
            found_in_inv = 1;
        }
    }

    if (found_in_inv == 0) {
        printf("В рюкзаке нет этого предмета\n");
    }
}
void write_diary(struct World *w, struct Player *p, char item_names[][MAX_NAME_LEN]) {
    if (!check_player(p)) return;
    FILE* file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл!\n");
        return;
    }
    fprintf(file, "=== День %d, Час %d ===\n", w->day, w->hour);
    fprintf(file, "Фермер: %s\n", p->name);
    fprintf(file, "Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] != 0) {
            fprintf(file, "Слот %d: %s\n", i, item_names[p->inventory[i]]);
        }
    }
    fclose(file);
    printf("Дневник обновлён\n");

}