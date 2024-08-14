#ifndef GET_SYS_H
#define GET_SYS_H

#include <stdio.h>

// Структура для хранения системной информации
typedef struct {
  char kernel[256];
  char os[256];
  char shell[256];
  char username[256];
  char device_name[256];
} SystemInfo;

// Функция для получения системной информации
SystemInfo get_system_info();

#endif // GET_SYS_H
