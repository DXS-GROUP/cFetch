// disk_info.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>

typedef struct {
  unsigned long long total_space;
  unsigned long long used_space;
} disk_info_t;

int get_disk_info(const char *path, disk_info_t *info) {
  struct statvfs stat;
  if (statvfs(path, &stat) != 0) {
    return 1; // Ошибка при получении информации о диске
  }

  info->total_space = (unsigned long long)stat.f_blocks * stat.f_frsize;
  info->used_space =
      (unsigned long long)(stat.f_blocks - stat.f_bfree) * stat.f_frsize;

  return 0; // Успешное получение информации о диске
}
