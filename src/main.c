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

#define RESET_BG "\033[0m"
#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"

#define RESET "\033[0m"
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

void wait_for_keypress()
{
#ifdef _WIN32
  _getch();
#else
  getchar();
#endif
}

void clear_terminal()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

char *create_repeated_string(char character, int count)
{
  char *result = (char *)malloc((count + 1) * sizeof(char));
  if (result == NULL)
  {
    printf("Memory error\n");
    return NULL;
  }
  memset(result, character, count);
  result[count] = '\0';
  return result;
}

void print_centered(const char *text)
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int terminal_width = w.ws_col;

  int text_length = strlen(text);
  // int padding = (terminal_width - text_length) / 2;
  int padding = 0;

  for (int i = 0; i < padding; i++)
  {
    putchar(' ');
  }
  printf("%s\n", text);
}

void print_centered_squares()
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int terminal_width = w.ws_col;

  const char *squares = WHITE
      " " RESET RED " " RESET GREEN " " RESET YELLOW " " RESET BLUE
      " " RESET MAGENTA " " RESET CYAN " " RESET WHITE " " RESET;

  int squares_length = 5 * 8;
  // int padding = (terminal_width - squares_length) / 2;
  int padding = 0;

  for (int i = 0; i < padding; i++)
  {
    putchar(' ');
  }
  printf("%s\n", squares);
}

void print_info(const char *label, const char *value, int *max_width)
{
  char buffer[100];
  const char *color;
  const char *icon;
  const char *spacer;

  icon = "";

  spacer = MAGENTA_BG " " RESET_BG;
  // spacer = "";

  if (strstr(label, "Usage"))
  {
    label = "";
    color = MAGENTA;
    icon = "";
    spacer = "";

    printf("%s%s%-10s " WHITE " %s%+40s%s", color, icon, label, WHITE, value,
           RESET);
  }
  else
  {
    if (strstr(label, "CPU"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "RAM"))
    {
      color = MAGENTA;
      icon = "󰍛  ";
    }
    else if (strstr(label, "Disk"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "Kernel"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "GPU"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "Uptime"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "OS"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "Host"))
    {
      color = MAGENTA;
      icon = "󰏔  ";
    }
    else if (strstr(label, "Shell"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else if (strstr(label, "WM"))
    {
      color = MAGENTA;
      icon = "  ";
    }
    else
    {
      color = RESET;
    }
    snprintf(buffer, sizeof(buffer), "%s%s%-10s " WHITE " %s %s%+40s%s", color,
             icon, label, WHITE, spacer, value, RESET);
    print_centered(buffer);

    int current_width = strlen(buffer);

    if (current_width > *max_width)
    {
      *max_width = current_width;
    }
  }
}

void print_memory_info(const char *label, int used_memory, int full_memory,
                       int *max_width)
{
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "%.2f GB of %.2f GB",
           (float)used_memory / 1024, (float)full_memory / 1024);
  print_info(label, buffer, max_width);
}

void print_disk_info(disk_info_t *info, int *max_width)
{
  if (get_disk_info("/", info) == 0)
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Used: %-1llu GB, Total: %-1llu GB",
             info->used_space / (1024 * 1024 * 1024),
             info->total_space / (1024 * 1024 * 1024));
    print_info("Disk", buffer, max_width);
  }
  else
  {
    printf("Failed to get disk information.\n");
  }
}

void print_uptime_info(uptime_t uptime, int *max_width)
{
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "%-1dd %-1dh %-1dm", uptime.days,
           uptime.hours, uptime.minutes);
  print_info("Uptime", buffer, max_width);
}

void print_window_manager(int *max_width)
{
  char buffer[100];
  const char *wm =
      getenv("DESKTOP_SESSION") ? getenv("DESKTOP_SESSION") : "Unknown";
  print_info("WM", wm, max_width);
}

void print_usage(const char *program_name, int *max_width)
{
  printf("%s ▄▄· ·▄▄▄▄▄▄ .▄▄▄▄▄ ▄▄·  ▄ .▄\n", RED);
  printf("▐█ ▌▪▐▄▄·▀▄.▀·•██  ▐█ ▌▪██▪▐█\n");
  printf("██ ▄▄██▪ ▐▀▀▪▄ ▐█.▪██ ▄▄██▀▐█\n");
  printf("▐███▌██▌.▐█▄▄▌ ▐█▌·▐███▌██▌▐▀\n");
  printf("·▀▀▀ ▀▀▀  ▀▀▀  ▀▀▀ ·▀▀▀ ▀▀▀ ·\n\n");

  printf(GREEN "Usage:\n\n--cpu\n--ram\n--gpu\n--disk\n--host\n--kernel\n--os\n--shell\n--uptime\n--colors\n--wm");
}

void print_os(int *max_width)
{
  FILE *os_file = fopen("/etc/os-release", "r");
  char buffer[100];
  char os_name[100] = "Unknown OS"; // Default value if not found
  size_t linecap = 0;
  char *line = NULL;

  if (!os_file)
  {
    perror("Failed to open /etc/os-release");
    return;
  }

  while (getline(&line, &linecap, os_file) != -1)
  {
    if (strncmp(line, "PRETTY_NAME=", 12) == 0)
    {
      snprintf(os_name, sizeof(os_name), "%s",
               line + 13);                    // Skip 'PRETTY_NAME="'
      os_name[strcspn(os_name, "\"")] = '\0'; // Remove trailing quote
      break;
    }
  }

  free(line);
  fclose(os_file);

  print_info("OS", os_name, max_width);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_art(int *max_width)
{
  const char *linux_logo_1 = RED_BG "    ,___  " RESET_BG;
  const char *linux_logo_2 = RED_BG "   _| ()\\\ " RESET_BG;
  const char *linux_logo_3 = RED_BG " /    --' " RESET_BG;
  const char *linux_logo_4 = RED_BG " \\\ __^/   " RESET_BG;

  print_centered(linux_logo_1);
  print_centered(linux_logo_2);
  print_centered(linux_logo_3);
  print_centered(linux_logo_4);
}

int main(int argc, char *argv[])
{
  clear_terminal();

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

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--cpu") == 0)
      flags[0] = 1;
    else if (strcmp(argv[i], "--ram") == 0)
      flags[1] = 1;
    else if (strcmp(argv[i], "--gpu") == 0)
      flags[2] = 1;
    else if (strcmp(argv[i], "--disk") == 0)
      flags[3] = 1;
    else if (strcmp(argv[i], "--host") == 0)
      flags[4] = 1;
    else if (strcmp(argv[i], "--kernel") == 0)
      flags[5] = 1;
    else if (strcmp(argv[i], "--os") == 0)
      flags[6] = 1;
    else if (strcmp(argv[i], "--shell") == 0)
      flags[7] = 1;
    else if (strcmp(argv[i], "--help") == 0)
      flags[8] = 1;
    else if (strcmp(argv[i], "--uptime") == 0)
      flags[9] = 1;
    else if (strcmp(argv[i], "--colors") == 0)
      flags[10] = 1;
    else if (strcmp(argv[i], "--wm") == 0)
      flags[11] = 1;
  }

  // print_art(&max_width);

  if (!flags[0] && !flags[1] && !flags[2] && !flags[3] && !flags[4] &&
      !flags[5] && !flags[6] && !flags[7] && !flags[8] && !flags[9] &&
      !flags[10] && !flags[11])
  {
    printf("\n");
    print_info("Kernel", sys_info.kernel, &max_width);
    print_info("Hostname", sys_info.device_name, &max_width);
    print_window_manager(&max_width);
    print_info("Shell", sys_info.shell, &max_width);
    print_uptime_info(uptime, &max_width);

    printf("\n");
    print_centered_squares(); // Center the colored squares
  }
  else
  {
    printf("\n");
    if (flags[0])
      print_info("CPU", cpu_info.model, &max_width);
    if (flags[1])
      print_memory_info("RAM", used_memory, full_memory, &max_width);
    if (flags[2])
      print_info("GPU", gpu_model, &max_width);
    if (flags[3])
      print_disk_info(&info, &max_width);
    if (flags[4])
      print_info("Hostname", sys_info.device_name, &max_width);
    if (flags[5])
      print_info("Kernel", sys_info.kernel, &max_width);
    if (flags[6])
      print_os(&max_width);
    if (flags[7])
      print_info("Shell", sys_info.shell, &max_width);
    if (flags[9])
      print_uptime_info(uptime, &max_width);
    if (flags[8])
      print_usage(argv[0], &max_width);
    if (flags[11])
      print_window_manager(&max_width);

    if (flags[10])
    {
      printf("\n");
      print_centered_squares(); // Center the colored squares
    }
  }

  char *squares = create_repeated_string(' ', max_width / 8);
  free(squares);

  // wait_for_keypress();

  return 0;
}
