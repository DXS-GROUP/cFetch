// gpu_info.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_gpu_model() {
  // Команда для получения информации о видеокарте
  FILE *fp;
  char path[1035];

  // Открываем команду для чтения
  fp = popen("lspci | grep -i vga", "r");
  if (fp == NULL) {
    return "Error.";
  }

  // Читаем вывод команды
  if (fgets(path, sizeof(path), fp) != NULL) {
    // Закрываем поток
    pclose(fp);

    // Находим начало названия видеокарты
    char *start = strstr(path, "[") + 1;

    // Находим конец названия видеокарты
    char *end = strstr(start, "]");

    // Выделяем память для нового строкового буфера
    char *gpu_model = (char *)malloc(end - start + 1);

    // Копируем название видеокарты в новый буфер
    strncpy(gpu_model, start, end - start);
    gpu_model[end - start] = '\0';

    return gpu_model;
  } else {
    pclose(fp);
    return "Error.";
  }
}
