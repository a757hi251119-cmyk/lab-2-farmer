#include <stdio.h>
#include <string.h>
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
