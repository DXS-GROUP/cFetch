#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  int days;
  int hours;
  int minutes;
} uptime_t;

uptime_t get_uptime() {
  FILE *fp;
  char buffer[256];
  uptime_t uptime;

  fp = fopen("/proc/uptime", "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening /proc/uptime\n");
    uptime.days = 0;
    uptime.hours = 0;
    uptime.minutes = 0;
    return uptime;
  }

  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    fprintf(stderr, "Error reading from /proc/uptime\n");
    fclose(fp);
    uptime.days = 0;
    uptime.hours = 0;
    uptime.minutes = 0;
    return uptime;
  }

  fclose(fp);

  double uptime_seconds = atof(buffer);
  int total_minutes = (int)(uptime_seconds / 60);

  uptime.days = total_minutes / (60 * 24);
  uptime.hours = (total_minutes / 60) % 24;
  uptime.minutes = total_minutes % 60;

  return uptime;
}
