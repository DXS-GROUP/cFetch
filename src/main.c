#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "get_data/disk_info.h"
#include "get_data/get_cpu.h"
#include "get_data/get_ram.h"
#include "get_data/get_sys.h"
#include "get_data/gpu_info.h"
#include "get_data/uptime.h"

#define RESET "\033[0m"
#define BLACK "\033[40m"
#define RED "\033[41m"
#define GREEN "\033[42m"
#define YELLOW "\033[43m"
#define BLUE "\033[44m"
#define MAGENTA "\033[45m"
#define CYAN "\033[46m"
#define WHITE "\033[47m"

#define SQUARE "     "

char *create_repeated_string(char character, int count) {
  char *result = (char *)malloc((count + 1) * sizeof(char));
  if (result == NULL) {
    printf("Memory error\n");
    return NULL;
  }

  for (int i = 0; i < count; i++) {
    result[i] = character;
  }
  result[count] = '\0';

  return result;
}

void print_info(const char *label, const char *value, int *max_width) {
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "  %-14s  %-23s", label, value);
  printf("%s\n", buffer);

  int current_width = strlen(buffer);
  if (current_width > *max_width) {
    *max_width = current_width;
  }
}

void print_memory_info(const char *label, int used_memory, int full_memory,
                       int *max_width) {
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "  %-14s  %-6.2f GB of %.2f GB", label,
           (float)used_memory / 1024, (float)full_memory / 1024);
  printf("%s\n", buffer);

  int current_width = strlen(buffer);
  if (current_width > *max_width) {
    *max_width = current_width;
  }
}

void print_disk_info(disk_info_t *info, int *max_width) {
  if (get_disk_info("/", info) == 0) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "  %-14s  %-1llu GB of %-1llu GB",
             "   Disk", info->used_space / (1024 * 1024 * 1024),
             info->total_space / (1024 * 1024 * 1024));
    printf("%s\n", buffer);

    int current_width = strlen(buffer);
    if (current_width > *max_width) {
      *max_width = current_width;
    }
  } else {
    printf("Не удалось получить информацию о диске.\n");
  }
}

void print_uptime_info(uptime_t uptime, int *max_width) {
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "  %-14s  %-1dd %-1dh %-1dm", "   Uptime",
           uptime.days, uptime.hours, uptime.minutes);
  printf("%s\n", buffer);

  int current_width = strlen(buffer);
  if (current_width > *max_width) {
    *max_width = current_width;
  }
}

void print_window_manager(int *max_width) {
  char buffer[100];
  const char *wm = getenv("DESKTOP_SESSION");

  snprintf(buffer, sizeof(buffer), "  %-14s  %-5s", "   WM", wm);
  printf("%s\n", buffer);
}

void print_usage(const char *program_name, int *max_width) {
  char buffer[200]; // Increased size
  snprintf(
      buffer, sizeof(buffer),
      "Usage: %s\n [--cpu] [--ram]\n [--gpu] [--disk]\n "
      "[--host] [--kernel]\n [--os] [--shell]\n [--uptime] [--colors]\n [--wm]",
      program_name);
  printf("%s\n", buffer);

  int current_width = strlen(buffer);
  if (current_width > *max_width) {
    *max_width = current_width;
  }
}

int main(int argc, char *argv[]) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int terminal_width = w.ws_col;

  CPUInfo cpu_info = get_cpu_info();
  SystemInfo sys_info = get_system_info();
  const char *gpu_model = get_gpu_model();
  disk_info_t info;
  uptime_t uptime = get_uptime();

  int used_memory = get_memory_usage();
  int full_memory = get_memory_total();

  int flags[12] = {0};

  int max_width = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--cpu") == 0) {
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
    } else if (strcmp(argv[i], "--help") == 0) {
      flags[8] = 1; // help_flag
    } else if (strcmp(argv[i], "--uptime") == 0) {
      flags[9] = 1; // uptime_flag
    } else if (strcmp(argv[i], "--colors") == 0) {
      flags[10] = 1; // colors_flag
    } else if (strcmp(argv[i], "--wm") == 0) {
      flags[11] = 1; // wm_flag
    }
  }

  if (!flags[0] && !flags[1] && !flags[2] && !flags[3] && !flags[4] &&
      !flags[5] && !flags[6] && !flags[7] && !flags[8] && !flags[9] &&
      !flags[10] && !flags[11]) {

    print_info("   Kernel", sys_info.kernel, &max_width);
    print_info("   Hostname", sys_info.device_name, &max_width);
    print_window_manager(&max_width);
    print_info("   Shell", sys_info.shell, &max_width);
    print_uptime_info(uptime, &max_width);

    printf("\n%s%s%s%s%s%s%s%s\n", BLACK SQUARE RESET, RED SQUARE RESET,
           GREEN SQUARE RESET, YELLOW SQUARE RESET, BLUE SQUARE RESET,
           MAGENTA SQUARE RESET, CYAN SQUARE RESET, WHITE SQUARE RESET);
    printf("\n");
  } else {
    if (flags[0])
      print_info("   CPU", cpu_info.model, &max_width);
    if (flags[1])
      print_memory_info("   RAM", used_memory, full_memory, &max_width);
    if (flags[2])
      print_info("   GPU", gpu_model, &max_width);
    if (flags[3])
      print_disk_info(&info, &max_width);
    if (flags[4])
      print_info("   Hostname", sys_info.device_name, &max_width);
    if (flags[5])
      print_info("   Kernel", sys_info.kernel, &max_width);
    if (flags[6])
      print_info("   OS", sys_info.os, &max_width);
    if (flags[7])
      print_info("   Shell", sys_info.shell, &max_width);
    if (flags[9])
      print_uptime_info(uptime, &max_width);
    if (flags[8])
      print_usage(argv[0], &max_width);
    if (flags[11])
      print_window_manager(&max_width);

    if (flags[10])
      printf("\n%s%s%s%s%s%s%s%s\n", BLACK SQUARE RESET, RED SQUARE RESET,
             GREEN SQUARE RESET, YELLOW SQUARE RESET, BLUE SQUARE RESET,
             MAGENTA SQUARE RESET, CYAN SQUARE RESET, WHITE SQUARE RESET);
    printf("\n");
  }

  char *squares = create_repeated_string(' ', max_width / 8);
  free(squares);

  return 0;
}
