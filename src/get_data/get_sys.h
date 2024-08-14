#ifndef GET_SYS_H
#define GET_SYS_H

#include <stdio.h>

typedef struct {
  char kernel[256];
  char os[256];
  char shell[256];
  char username[256];
  char device_name[256];
} SystemInfo;

SystemInfo get_system_info();

#endif // GET_SYS_H
