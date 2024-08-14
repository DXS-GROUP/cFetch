#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_gpu_model() {
  FILE *fp;
  char path[1035];

  fp = popen("lspci | grep -i vga", "r");
  if (fp == NULL) {
    return "Error.";
  }

  if (fgets(path, sizeof(path), fp) != NULL) {
    pclose(fp);

    char *start = strstr(path, "[") + 1;

    char *end = strstr(start, "]");

    char *gpu_model = (char *)malloc(end - start + 1);

    strncpy(gpu_model, start, end - start);
    gpu_model[end - start] = '\0';

    return gpu_model;
  } else {
    pclose(fp);
    return "Error.";
  }
}
