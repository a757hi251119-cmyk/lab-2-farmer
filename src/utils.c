#include <stdio.h>
#include "utils.h"
#include "player.h"

int durak(char message[]) {
    int x;
    printf("%s", message);
    while (scanf("%d", &x) != 1) {
        printf("Ошибка! Введите число: ");
        while (getchar() != '\n');
    }
    return x;
}

int check_player(struct Player *p){
    if (p == NULL) {
        printf("Сначала создайте персонажа!\n");
        return 0;
     }
    return 1;
}
void show_time(struct World* w) {
    printf("\n===Текущее время===\n");
    printf("День: %d, %02d:00\n", w->day, w->hour);
}
void change_time(struct World* w) {
    int add_h = durak("Сколько часов прибавить?\n");
    w->hour += add_h;
    if (w->hour >= 24) {
        w->day += (w->hour / 24);
        w->hour = w->hour % 24;
    }
}
