#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "get_data/disk_info.h"
#include "get_data/get_cpu.h"
#include "get_data/get_ram.h"
#include "get_data/get_sys.h"
#include "get_data/gpu_info.h"

void print_info(const char *label, const char *value) {
  printf(" %-14s  %-23s \n", label, value);
}

void print_memory_info(const char *label, int used_memory, int full_memory) {
  printf(" %-14s  %-6.2f GB of %.2f GB \n", label, (float)used_memory / 1024,
         (float)full_memory / 1024);
}

void print_disk_info(disk_info_t *info) {
  if (get_disk_info("/", info) == 0) {
    printf(" %-14s  %-1llu GB of %-1llu GB \n", "   Disk",
           info->used_space / (1024 * 1024 * 1024),
           info->total_space / (1024 * 1024 * 1024));
  } else {
    printf("Не удалось получить информацию о диске.\n");
  }
}

int main(int argc, char *argv[]) {
  CPUInfo cpu_info = get_cpu_info();
  SystemInfo sys_info = get_system_info();
  const char *gpu_model = get_gpu_model();
  disk_info_t info;

  int used_memory = get_memory_usage();
  int full_memory = get_memory_total();

  int flags[9] = {
      0}; // 0: cpu_flag, 1: ram_flag, 2: gpu_flag, 3: disk_flag, 4: user_flag,
          // 5: kernel_flag, 6: os_flag, 7: shell_flag, 8: help_flag

  // Обработка аргументов командной строки
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) {
      flags[8] = 1; // help_flag
    } else if (strcmp(argv[i], "--cpu") == 0) {
      flags[0] = 1; // cpu_flag
    } else if (strcmp(argv[i], "--ram") == 0) {
      flags[1] = 1; // ram_flag
    } else if (strcmp(argv[i], "--gpu") == 0) {
      flags[2] = 1; // gpu_flag
    } else if (strcmp(argv[i], "--disk") == 0) {
      flags[3] = 1; // disk_flag
    } else if (strcmp(argv[i], "--host") == 0) {
      flags[4] = 1; // user_flag
    } else if (strcmp(argv[i], "--kernel") == 0) {
      flags[5] = 1; // kernel_flag
    } else if (strcmp(argv[i], "--os") == 0) {
      flags[6] = 1; // os_flag
    } else if (strcmp(argv[i], "--shell") == 0) {
      flags[7] = 1; // shell_flag
    }
  }

  // Если не указаны флаги, выводим все данные
  if (!flags[0] && !flags[1] && !flags[2] && !flags[3] && !flags[4] &&
      !flags[5] && !flags[6] && !flags[7] && !flags[8]) {
    print_info("   Kernel", sys_info.kernel);
    print_info("   Hostname", sys_info.device_name);
    print_info("   Shell", sys_info.shell);
  } else {
    // Выводим текст указанных параметров
    if (flags[0])
      print_info("   CPU", cpu_info.model);
    if (flags[1])
      print_memory_info("   RAM", used_memory, full_memory);
    if (flags[2])
      print_info("   GPU", gpu_model);
    if (flags[3])
      print_disk_info(&info);
    if (flags[4])
      print_info("   Hostname", sys_info.device_name);
    if (flags[5])
      print_info("   Kernel", sys_info.kernel);
    if (flags[6])
      print_info("   OS", sys_info.os);
    if (flags[7])
      print_info("   Shell", sys_info.shell);
    if (flags[8]) {
      printf("Usage: %s [--cpu] [--ram] [--gpu] [--disk] [--host] "
             "[--kernel] [--os] [--shell]\n",
             argv[0]);
    }
  }

  return 0;
}
