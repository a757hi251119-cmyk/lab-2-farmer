#include <stdio.h>
#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_HOUR 8
const char* item_names[] = {
    "Пусто", "Дерево", "Камень", "Семена",
    "Железо", "Золото", "Алмаз", "Трава", "Вода", "Уголь"
};
int main() {
    int current_day = 1;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 4, 1};
    int choice;
    int slot;
    int obj;
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
            printf("Сколько часов прибавить?\n");
            int add_h;
            scanf("%d", &add_h);
            current_hour += add_h;
            if (current_hour >= 24) {
                current_day += (current_hour / 24);
                current_hour = current_hour % 24;
            }            
            break;
        case 3:
            printf("\n===Инвентарь===\n");
            for (int i = 0; i < 10; i++) {
                printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
            }
            break;
        case 4:
            printf("Куда положить?\n");
            scanf("%d", &slot);
            if (slot<0 || slot >= INVENTORY_SIZE) {
                printf("Нет такого слота\n");
                break;
            }
            printf("Что положить?\n");
            scanf("%d", &obj);
            inventory[slot] = obj;
            printf("Добавлено %s", item_names[inventory[slot]]);
            break;
        case 5:
            printf("Что выбросить?\n");
            scanf("%d", &slot);
            if (slot < 0 || slot >= INVENTORY_SIZE) {
                printf("Нет такого слота\n");
                break;
            }
            inventory[slot] = 0;
            printf("Слот очищен\n");
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
