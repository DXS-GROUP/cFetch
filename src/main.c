#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "get_data/get_cpu.h"
#include "get_data/get_gpu.h"
#include "get_data/get_ram.h"
#include "get_data/get_sys.h"

int main(int argc, char *argv[]) {
  CPUInfo cpu_info = get_cpu_info();
  SystemInfo sys_info = get_system_info();
  long used_memory = get_memory_usage();

  // Флаги для отслеживания переданных параметров
  int cpu_flag = 0;
  int ram_flag = 0;
  int gpu_flag = 0;
  int disk_flag = 0;
  int user_flag = 0;
  int net_flag = 0;
  int kernel_flag = 0;
  int os_flag = 0;
  int shell_flag = 0;

  // Обработка аргументов командной строки
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--cpu") == 0) {
      cpu_flag = 1;
    } else if (strcmp(argv[i], "--ram") == 0) {
      ram_flag = 1;
    } else if (strcmp(argv[i], "--gpu") == 0) {
      gpu_flag = 1;
    } else if (strcmp(argv[i], "--disk") == 0) {
      disk_flag = 1;
    } else if (strcmp(argv[i], "--user") == 0) {
      user_flag = 1;
    } else if (strcmp(argv[i], "--net") == 0) {
      net_flag = 1;
    } else if (strcmp(argv[i], "--kernel") == 0) {
      kernel_flag = 1;
    } else if (strcmp(argv[i], "--os") == 0) {
      os_flag = 1;
    } else if (strcmp(argv[i], "--shell") == 0) {
      shell_flag = 1;
    }
  }

  // Если параметры не указаны, выводим значения по умолчанию
  if (!cpu_flag && !ram_flag && !gpu_flag && !disk_flag && !user_flag &&
      !net_flag && !kernel_flag && !os_flag && !shell_flag) {
    printf("Kernel: %s\n", sys_info.kernel);
    // printf("OS: %s\n", sys_info.os);
    printf("Shell: %s\n", sys_info.shell);
    printf("User: %s\n", sys_info.username);
  } else {
    // Выводим текст указанных параметров
    if (cpu_flag)
      printf("CPU: %s\n", cpu_info.model);
    if (ram_flag)
      if (used_memory == -1) {
        printf("RAM: Unable to get information.\n");
      } else {
        printf("RAM: %.2f MB\n", (float)used_memory / (1024 * 1024));
      }
    if (gpu_flag)
      printf("GPU ");
    if (disk_flag)
      printf("Disk ");
    if (user_flag)
      printf("User: %s\n", sys_info.username);
    if (net_flag)
      printf("Net ");
    if (kernel_flag)
      printf("Kernel: %s\n", sys_info.kernel);
    if (os_flag)
      printf("OS: %s\n", sys_info.os);
    if (shell_flag)
      printf("Shell: %s\n", sys_info.shell);
    printf("\n");
  }

  return 0;
}
