#include "player.h"
#include "utils.h"
#include "map.h"
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
    struct Player* pl = NULL;
    struct Map* m = NULL;
    struct World world = { 1, START_HOUR };
    int choice;
    char item_names[10][32];
    srand(time(NULL));
    load_items(item_names);
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
            create_player(&pl);
            break;
        case 2:
            show_player_card(pl);
            break;
        case 3:
            show_time(&world);
            break;
        case 4:
            change_time(&world);
            break;
        case 5:
            show_inventory(pl, item_names);
            break;
        case 6:
            put_item(pl, item_names);
            break;
        case 7:
            delete_item(pl, item_names);
            break;
        case 8:
            delate_all_items(pl, item_names);
            break;
        case 9:
            find_item(pl, item_names);
            break;
        case 10:
            write_diary(&world, pl, item_names);
            break;
        case 11:
            process_logs();
            break;
        case 12:
            if (pl == NULL) {
                printf("Сначала создайте персонажа!\n");
                break;
            }

            struct Map map;
            map.floor = 1;
            generate_map(&map, pl);
            char move;
            int wrap_count = 0;
            int portal_spawned = 0;
            int new_x, new_y;

            while (true) {
                printf("HP: %d/%d | Стамина: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
                    pl->hp, pl->max_hp, pl->stamina, pl->max_stamina, pl->gold, map.floor, world.hour);
                print_map(&map);
                printf("Куда идём? (W/A/S/D, Q - выход): ");
                scanf(" %c", &move);
                if (move == 'Q') {
                    break;
                }
                if (move == 'W') {
                    new_x = pl->x;
                    new_y = pl->y - 1;
                }
                if (move == 'A') {
                    new_x = pl->x - 1;
                    new_y = pl->y;
                }
                if (move == 'S') {
                    new_x = pl->x;
                    new_y = pl->y + 1;
                }
                if (move == 'D') {
                    new_x = pl->x + 1;
                    new_y = pl->y;
                }
                if (new_x >= map.width || new_y >= map.height || new_y < 0 || new_x < 0) {
                    wrap_count++;
                }
                new_x = (new_x + map.width) % map.width;
                new_y = (new_y + map.height) % map.height;

                if (wrap_count >= 3 && portal_spawned == 0) {
                    while (true) {
                        int px = rand() % map.width;
                        int py = rand() % map.height;
                        if (map.map[py][px] == EMPTY && px != pl->x && py != pl->y) {
                            map.map[py][px] = PORTAL;
                            portal_spawned = 1;
                            break;
                        }
                    }
                }
                if (map.map[new_y][new_x] == TREE || map.map[new_y][new_x] == ROCK) {
                    printf("Туда нельзя!\n");
                    map.map[pl->y][pl->x] = HERO;
                }
                else if (map.map[new_y][new_x] == PORTAL) {
                    for (int i = 0; i < map.height; i++) {
                        free(map.map[i]);
                    }
                    free(map.map);
                    map.floor++;
                    pl->gold += 10 * map.floor;
                    world.hour += 1; 
                    if (world.hour >= 24) {
                        world.day++;
                        world.hour %= 24;
                    }
                    pl->stamina -= 5;

                    generate_map(&map, pl);
                    wrap_count = 0;
                    portal_spawned = 0;
                    continue;
                }
                else {
                    map.map[pl->y][pl->x] = EMPTY;
                    pl->x = new_x;
                    pl->y = new_y;
                    map.map[pl->y][pl->x] = HERO;
                }
                pl->stamina -= 1;
                if (pl->stamina <= 0 || (world.hour >= 2 && world.hour < 8)) {
                    printf("Вы пали без сил! Эвакуация...\n");
                    pl->gold /= 2;
                    break;
                }
            }
            break;
        default:
            printf("Неверный ввод\n");

        }
    }
        while (choice != 0);
        return 0;
}
    