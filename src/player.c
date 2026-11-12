#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

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
    if (p != NULL) {
        printf("Сначала создайте персонажа!\n");
        return;
    }
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