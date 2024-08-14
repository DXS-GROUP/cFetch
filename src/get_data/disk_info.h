// disk_info.h
#ifndef DISK_INFO_H
#define DISK_INFO_H

typedef struct {
  unsigned long long total_space;
  unsigned long long used_space;
} disk_info_t;

int get_disk_info(const char *path, disk_info_t *info);

#endif // DISK_INFO_H
