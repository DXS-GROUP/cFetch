#ifndef UPTIME_H
#define UPTIME_H

typedef struct {
  int days;
  int hours;
  int minutes;
} uptime_t;

uptime_t get_uptime();

#endif // UPTIME_H
