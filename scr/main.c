#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define HOURS_IN_DAY 24
#define START_HOUR 8
#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30
#define WALL '#'
#define EMPTY '.'
#define HERO '@'
#define TREE 'T'
#define ROCK 'R'
#define PORTAL 'P'

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

char** generate_map(int *height, int *width, int *hero_x, int *hero_y) {
    *height = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
    *width = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
    char **map = (char**)malloc(*height * sizeof(char*));
    for (int i = 0; i < *height; i++) {
        map[i] = (char*)malloc(*width * sizeof(char));
    }
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            if (i>0 && 0 < i < *height - 1 && j > 0 && 0 < j < *width - 1) {
                if (rand() % 100 < 20) {
                    if (rand() % 2 == 0) {
                        map[i][j] = ROCK;
                    }
                    else {
                        map[i][j] = TREE;
                    }
                }
                else {
                    map[i][j] = EMPTY;
                }
            }
            else {
                map[i][j] = EMPTY;
            }
        }
    }
    while (true) {
        *hero_x = rand() % *width;
        *hero_y = rand() % *height;
        if (map[*hero_y][*hero_x] == EMPTY) {
            map[*hero_y][*hero_x] = HERO;
            break;
        }
    }
    return map;
}

void print_map(char** map, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", map[i][j]);
        }
    printf("\n");
    }
}


int main() {
    struct Player* pl = NULL;
    struct Map* map = NULL;
    int choice;
    char item_names[10][32];
    srand(time(NULL));
    load_items(item_names);
    farmer_name[strcspn(farmer_name, "\n")] = '\0';

    do {
        printf("\n=== Супер весёлый фермер ===\n");
        printf("[0] Выход\n");
        printf("[1] Создать / пересоздать персонажа\n");
        printf("[2] Карточка персонажа\n");
        printf("[3] Посмотреть на часы\n");
        printf("[4] Промотать время\n");
        printf("[5] Посмотреть инвентарь\n");
        printf("[6] Положить предмет в слот\n");
        printf("[7] Выбросить предмет\n");
        printf("[8] Очистить от мусора\n");
        printf("[9] Найти предмет\n");
        printf("[10] Записать в дневник\n");
        printf("[11] Анализатор логов\n");
        printf("[12] Навстречу приключениям!\n");
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
        case 10:
            int height, width;
            int hero_x, hero_y;
            char move;
            int new_y, new_x;
            int wrap_count = 0;
            int portal_spawned = 0;
            char** map = generate_map(&height, &width, &hero_x, &hero_y);
            if (map == NULL){ 
                printf("Ошибка памяти\n");
                break;
            }
            while (true) {
                print_map(map, height, width);
                printf("Куда идем? (W/A/S/D, Q - выход): ");
                scanf(" %c", &move);
                if (move == 'Q') {
                    break;
                }
                if (move == 'W') {
                    new_x = hero_x;
                    new_y = hero_y -1;
                }
                if (move == 'A') {
                    new_x = hero_x -1;
                    new_y = hero_y;
                }
                if (move == 'S') {
                    new_x = hero_x;
                    new_y = hero_y + 1;
                }
                if (move == 'D') {
                    new_x = hero_x +1;
                    new_y = hero_y;
                }
                if (new_x >= width || new_y >= height || new_y < 0 || new_x < 0) {
                    wrap_count++;
                }
                new_x = (new_x + width) % width;
                new_y = (new_y + height) % height;
                if (wrap_count >= 3 && portal_spawned == 0) {
                    while (true) {
                        int px = rand() % width;
                        int py = rand() % height;
                        if (map[py][px] == EMPTY && px != hero_x && py != hero_y) {
                            map[py][px] = PORTAL;
                            portal_spawned = 1;
                            printf("DEBUG: проверка карты в точке: %c\n", map[py][px]);
                            break;
                        }
                    }
                }
                if (map[new_y][new_x] == TREE || map[new_y][new_x] == ROCK) {
                     printf("Туда нельзя!\n");
                     map[hero_y][hero_x] = HERO;
                }
                else if (map[new_y][new_x] == PORTAL) {
                    for (int i = 0; i < height; i++) {
                        free(map[i]);
                    }
                    free(map);
                    map = generate_map(&height, &width, &hero_x, &hero_y);
                    wrap_count = 0;
                    portal_spawned = 0;
                    continue;
                }
                else {
                     map[hero_y][hero_x] = EMPTY;
                     hero_x = new_x;
                     hero_y = new_y;
                     map[hero_y][hero_x] = HERO;
                }
            }
         
            for (int i = 0; i < height; i++) {
                free(map[i]);
            }
            free(map);
            break;
        default:
            printf("Неверный ввод\n");
        }
    } 
    while (choice != 0);
    return 0;
}
