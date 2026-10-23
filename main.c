#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define HOURS_IN_DAY 24
#define START_HOUR 8
char item_names[MAX_ITEMS][MAX_NAME_LEN];
int durak(char message[]) {
    int x;
    printf("%s", message);
    while (scanf("%d", &x) != 1) {
        printf("Ошибка! Введите число: ");
        while (getchar() != '\n');
    }
    return x;
}

void load_items(char item_names[][MAX_NAME_LEN]) {
    FILE* file = fopen("items.txt", "r");
    char default_names[10][32] =
    { "Empty", "Tree", "Stone", "Seeds", "Iron",
      "Gold", "Diamond", "Grass", "Water", "Coal" };
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл!\n");
        for (int i = 0; i < 10; i++) {
            strcpy(item_names[i], default_names[i]);
        }
    }
    else {
        for (int i = 0; i < MAX_ITEMS; i++) {
            fgets(item_names[i], MAX_NAME_LEN, file);
            item_names[i][strcspn(item_names[i], "\n")] = '\0';            
        }
        fclose(file);
    }

}
void show_time(int day, int hour) {
    printf("\n===Текущее время===\n");
    printf("День: %d, %02d:00\n", day, hour);
}
void change_time(int* day, int* hour) {
    int add_h = durak("Сколько часов прибавить?\n");
    *hour += add_h;
    if (*hour >= 24) {
        *day += (*hour / 24);
        *hour = *hour % 24;
    }
}
void show_inventory(int inventory[], char item_names[][MAX_NAME_LEN]) {
    printf("\n===Инвентарь===\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
    }
}
void put_item(int inventory[],  char item_names[][MAX_NAME_LEN]) {
    int slot = durak("Куда положить?\n");
    if (slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Нет такого слота\n");
        return;
    }
    int obj = durak("Что положить\n");
    inventory[slot] = obj;
    printf("Добавлено %s", item_names[inventory[slot]]);
}
void delete_item(int inventory[]) {
    int slot = durak("Что выбросить?\n");
    if (slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Нет такого слота\n");
        return;
    }
    inventory[slot] = 0;
    printf("Слот очищен\n");
}
void delate_all_items(int inventory[], char item_names[][MAX_NAME_LEN]) {
    int obj = durak("От какого предмета очистить инвентарь?\n");
    int count_slot = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == obj) {
            inventory[i] = 0;
            count_slot++;
        }
    }
    printf("Было очищено %d слотов от %s", count_slot, item_names[obj]);
}
void find_item(int inventory[], char item_names[][MAX_NAME_LEN]) {
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
        if (inventory[i] == found_id) {
            printf("Найден в слоте %d\n", i);
            found_in_inv = 1;
        }
    }

    if (found_in_inv == 0) {
        printf("В рюкзаке нет этого предмета\n");
    }
}
void write_diary(int current_day, int current_hour, int inventory[], char item_names[][MAX_NAME_LEN], char farmer_name[]) {
    FILE* file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл!\n");
        return;
    }
    fprintf(file, "=== День %d, Час %d ===\n", current_day, current_hour);
    fprintf(file, "Фермер: %s\n", farmer_name);
    fprintf(file, "Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] != 0) {
            fprintf(file, "Слот %d: %s\n", i, item_names[inventory[i]]);
        }
    }
    fclose(file);
    printf("Дневник обновлён\n");

}
void process_logs() {
    FILE* inp = fopen("input.txt", "r");
    FILE* out = fopen("outputt.txt", "w");
    if (inp == NULL || out == NULL) { 
        printf("Ошибка: не удалось открыть файл!\n");
        return;
    }
    int count_info = 0;
    int count_warn = 0;
    int count_error = 0;
    int line_num = 1;
    char buffer[256];
    while (fgets(buffer, 256, inp) != NULL) {
        if (strncmp(buffer, "[INFO]", 6) == 0) {
            count_info++;
        }
        if (strncmp(buffer, "[WARN]", 6) == 0) {
            count_warn++;
            fprintf(out, "Строка %d: %s\n", line_num, buffer + 7);
        }
        if (strncmp(buffer, "[ERROR]", 7) == 0) {
            count_error++;
            fprintf(out, "Строка %d: %s\n", line_num, buffer + 8);
        }
        line_num++;
    }
    fprintf(out, "INFO: %d\n", count_info);
    fprintf(out, "WARN: %d\n", count_warn);
    fprintf(out, "ERROR: %d", count_error);
    printf("INFO: %d | WARN: %d | ERROR: %d", count_info, count_warn, count_error);
    fclose(inp);
    fclose(out);
}
int main() {
    int current_day = 1;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 4, 1};
    int choice;
    int slot;
    int obj;
    char item_names[10][32];
    char farmer_name[32];
    load_items(item_names);
    printf("Введите имя: ");
    fgets(farmer_name, 32, stdin);
    farmer_name[strcspn(farmer_name, "\n")] = '\0';

    do {
        printf("\n=== Фермер %s ===\n", farmer_name);
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Очистить от мусора\n");
        printf("[7] Найти предмет\n");
        printf("[8] Записать в дневник\n");
        printf("[9] Анализатор логов\n");
        printf("Выбор: ");
        while (scanf("%d", &choice) != 1) {
            printf("Ошибка! Введите число: ");
            while (getchar() != '\n');
        }
        switch (choice) {
        case 0:
            printf("Выход\n");
            break;
        case 1:
            show_time(current_day, current_hour);
            break;
        case 2:
            change_time(&current_day, &current_hour);
            break;
        case 3:
            show_inventory(inventory, item_names);
            break;
        case 4:
            put_item(inventory, item_names);
            break;
        case 5:
            delete_item(inventory);            
            break;
        case 6:
            delate_all_items(inventory, item_names);
            break;
        case 7:
            find_item(inventory, item_names);
            break;
        case 8:
            write_diary(current_day, current_hour, inventory, item_names, farmer_name);
            break;
        case 9:
            process_logs();
            break;
        default:
            printf("Неверный ввод\n");
        }
    } while (choice != 0);
    return 0;
}
