#include "strings.h"
#include <stdio.h>

size_t slen(char *mass) {
  size_t count = 0;
  while (*mass != '\0') {
    count++;
    mass++;
  }
  return count;
}

void scpy(char *mass, char *mass2) {
  while (*mass != '\0') {
    *mass2++ = *mass++;
  }
  *mass = '\0';
}

char *sspn(char *mass, char element) {
  for (; *mass != '\0'; mass++) {
    if (*mass == element) {
      return mass;
    }
  }
  return NULL;
}

int stok(char *src, char symb, char **strs) {
  int i = 0;
  int numstrs = 1;
  strs[i] = src;
  while (src[i] != '\0') {
    if (src[i] == symb) {
      src[i] = '\0';
      strs[numstrs] = &(src[i]) + 1;
      numstrs++;
    }
    i++;
  }
  return numstrs;
}

void unstok(char *str, char delim, char **ptr, int cnt) {
  for (int i = 1; i < cnt; i++) {
    *(ptr[i] - 1) = delim;
  }
}

int scmp(char *mass, char *mass2) {
  int len = slen(mass);
  for (int i = 0; i < len; i++) {
    if (mass[i] != mass2[i]) {
      return mass[i] - mass2[i];
    }
  }
  return 0;
}

char min_to_max(char element) { return ((int)element - 32); }