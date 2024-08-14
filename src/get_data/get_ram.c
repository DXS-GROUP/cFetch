#include "get_ram.h"
#include <stdlib.h>
#include <string.h>

long get_memory_usage() {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (!meminfo) {
    return -1; // Ошибка открытия файла
  }

  long total_memory = 0;
  long free_memory = 0;
  long buffers = 0;
  long cached = 0;
  char buffer[256];

  while (fgets(buffer, sizeof(buffer), meminfo)) {
    if (strncmp(buffer, "MemTotal:", 9) == 0) {
      total_memory = atol(buffer + 10); // Значение в кБ
    } else if (strncmp(buffer, "MemFree:", 8) == 0) {
      free_memory = atol(buffer + 9); // Значение в кБ
    } else if (strncmp(buffer, "Buffers:", 8) == 0) {
      buffers = atol(buffer + 9); // Значение в кБ
    } else if (strncmp(buffer, "Cached:", 7) == 0) {
      cached = atol(buffer + 8); // Значение в кБ
    }
  }

  fclose(meminfo);

  // Вычисляем использованную память
  long used_memory = total_memory - free_memory - buffers - cached;

  // Возвращаем использованную память в мегабайтах
  return used_memory / 1024; // Переводим в мегабайты
}

long get_memory_total() {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if (!meminfo) {
    return -1; // Ошибка открытия файла
  }

  long total_memory = 0;
  long free_memory = 0;
  long buffers = 0;
  long cached = 0;
  char buffer[256];

  while (fgets(buffer, sizeof(buffer), meminfo)) {
    if (strncmp(buffer, "MemTotal:", 9) == 0) {
      total_memory = atol(buffer + 10); // Значение в кБ
    } else if (strncmp(buffer, "MemFree:", 8) == 0) {
      free_memory = atol(buffer + 9); // Значение в кБ
    } else if (strncmp(buffer, "Buffers:", 8) == 0) {
      buffers = atol(buffer + 9); // Значение в кБ
    } else if (strncmp(buffer, "Cached:", 7) == 0) {
      cached = atol(buffer + 8); // Значение в кБ
    }
  }

  fclose(meminfo);

  // Вычисляем использованную память
  long used_memory = total_memory - free_memory - buffers - cached;

  // Возвращаем использованную память в мегабайтах
  return total_memory / 1024; // Переводим в мегабайты
}
