#include <stdio.h>
#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_HOUR 8
int main() {
    int current_day = 1;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 4, 1 };
    int choice;
    do {
        printf("\n=== МЕНЮ ===\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Очистить от мусора\n");
        printf("Выбор: ");
        scanf("%d", &choice);
        switch (choice) {
        case 0:
            printf("Выход\n");
            break;
        case 1:
            printf("\n===Текущее время===\n");
            printf("День: %d, %02d:00\n", current_day, current_hour);
            break;
        case 2:
            printf("Время\n");
            break;
        case 3:
            printf("Инвентарь\n");
            break;
        case 4:
            printf("Положить\n");
            break;
        case 5:
            printf("Выбросить\n");
            break;
        case 6:
            printf("Очистка\n");
            break;
        default:
            printf("Неверный ввод\n");
        }
    } while (choice != 0);
    return 0;
}
