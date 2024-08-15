#include "get_cpu.h"
#include <stdlib.h>
#include <string.h>

CPUInfo get_cpu_info()
{
  CPUInfo cpu_info;

  FILE *fp =
      popen("awk -F ':' '/model name/ {print $2; exit}' /proc/cpuinfo", "r");
  if (fp == NULL)
  {
    strcpy(cpu_info.model, "Unknown");
    cpu_info.load = -1.0;
    return cpu_info;
  }

  fgets(cpu_info.model, sizeof(cpu_info.model), fp);
  pclose(fp);

  cpu_info.model[strcspn(cpu_info.model, "\n")] = 0;

  FILE *load_fp = popen("top -bn1 | grep 'Cpu(s)' | sed 's/.*, "
                        "*\\([0-9.]*\\)%* id.*/\\1/' | awk '{print 100 - $1}'",
                        "r");
  if (load_fp == NULL)
  {
    cpu_info.load = -1.0;
    return cpu_info;
  }

  fscanf(load_fp, "%f", &cpu_info.load);
  pclose(load_fp);

  return cpu_info;
}
