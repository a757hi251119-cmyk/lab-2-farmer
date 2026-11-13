#include "player.h"
#include "utils.h"
#include "map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    struct Player* pl = NULL;
    struct Map* m = NULL;
    struct World world = { 1, START_HOUR };
    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    int choice;
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
        printf("[13] Лечь спать\n");
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
            if (!check_player(pl)) break;
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
            if (!check_player(pl)) break;
            struct Map map;
            map.floor = 1;
            generate_map(&map, pl);
            char move;
            int wrap_count = 0;
            int portal_spawned = 0;
            int new_x, new_y;

            while (true) {
                const char* relic_name = "Нет";
                if (pl->active_relic == 1) relic_name = "Кольцо жадности";
                if (pl->active_relic == 2) relic_name = "Сапоги-скороходы";
                if (pl->active_relic == 3) relic_name = "Камень тролля";

                printf("HP: %d/%d | Стамина: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00 | Амулет: %s\n",
                    pl->hp, pl->max_hp, pl->stamina, pl->max_stamina, pl->gold, map.floor, world.hour, relic_name);
                print_map(&map);
                printf("Куда идём? (W/A/S/D, Q - выход): ");
                scanf(" %c", &move);
                if (move == 'Q') {
                    free_map(&map);
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
                    free_map(&map);
                    map.floor++;
                    int gold_reward = 10 * map.floor;
                    if (pl->active_relic == 1) gold_reward *= 2; // Кольцо жадности
                    pl->gold += gold_reward;
                    world.hour += 1; 
                    if (world.hour >= 24) {
                        world.day++;
                        world.hour %= 24;
                    }

                    int stamina_cost_floor = 5;
                    if (pl->active_relic == 2) stamina_cost_floor = 1; // Сапоги-скороходы
                    pl->stamina -= stamina_cost_floor;

                    if (pl->stamina <= 0) {
                        printf("Вы пали без сил! Эвакуация...\n");
                        pl->gold /= 2;
                        free_map(&map);
                        break;
                    }

                    if (rand() % 100 < 20) { 
                        int new_relic = rand() % 3 + 1;
                        printf("Вы нашли реликвию! Надеть (1) или продать за 25 золота (2)? ");
                        int choice = durak("");
                        if (choice == 1) {
                            pl->active_relic = new_relic;
                        }
                        else {
                            pl->gold += 25;
                        }
                    }

                    generate_map(&map, pl);
                    wrap_count = 0;
                    portal_spawned = 0;
                    
                }
                else {
                    map.map[pl->y][pl->x] = EMPTY;
                    pl->x = new_x;
                    pl->y = new_y;
                    map.map[pl->y][pl->x] = HERO;
                }

                int stamina_cost = 1;
                if (pl->active_relic == 1) stamina_cost = 2; // Кольцо жадности
                pl->stamina -= stamina_cost;

                if (pl->active_relic == 3) { // Камень тролля
                    pl->hp += 1;
                    if (pl->hp > pl->max_hp) pl->hp = pl->max_hp;
                }
                if (pl->stamina <= 0 || (world.hour >= 2 && world.hour < 8)) {
                    printf("Вы пали без сил! Эвакуация...\n");
                    pl->gold /= 2;
                    free_map(&map);
                    break;
                }
            }
            break;
        case 13:
            if (!check_player(pl)) break;
            world.day++;
            world.hour = 8;
            pl->hp = pl->max_hp;
            pl->stamina = pl->max_stamina;
            printf("Вы выспались! День %d, 08:00\n", world.day);
            break;
        default:
            printf("Неверный ввод\n");

        }
    }
        while (choice != 0);
        if (pl != NULL) {
            printf("Память структуры по адресу %p освобождена\n", (void*)pl);
            free(pl);
            pl = NULL;
        }
        return 0;
}
    