#ifndef GET_CPU_H
#define GET_CPU_H

#include <stdio.h>

typedef struct {
  char model[256];
  float load;
} CPUInfo;

CPUInfo get_cpu_info();

#endif // GET_CPU_H
