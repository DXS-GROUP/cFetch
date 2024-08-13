#ifndef GET_CPU_H
#define GET_CPU_H

#include <stdio.h>

// Структура для хранения информации о процессоре
typedef struct {
  char model[256];
  float load;
} CPUInfo;

// Функция для получения информации о процессоре
CPUInfo get_cpu_info();

#endif // GET_CPU_H
