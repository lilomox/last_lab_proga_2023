#include <stddef.h>

size_t slen(char *mass);

void scpy(char *mass, char *mass2);

char *sspn(char *mass, char element);

int stok(char *src, char symb, char **strs);

void unstok(char *str, char delim, char **ptr, int cnt);

int scmp(char *mass, char *mass2);

char min_to_max(char element);