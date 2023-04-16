#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

#define MAX_PATH 260

typedef struct path_to {
  char string[MAX_PATH];
  size_t len;
} path_to;

typedef enum PathError {
  ErrorTypeNon,
  ErrorTypeAintPath,
  ErrorTypeTooLongLenght,
  ErrorTypeBannedSymbol,
  ErrorTypeVagueSymbol,
  ErrorTypeIncorrectPath
} PathError;

typedef struct errors_of_path {
  char *column;
  PathError type_of_error;
} errors_of_path;

path_to *path_to_init() {
  path_to *new = (path_to *)malloc(sizeof(path_to));
  if (new == NULL) {
    return NULL;
  }
  new->len = 0;
  return new;
}

void path_to_add(path_to *path, char value) {
  path->string[path->len] = value;
  path->len++;
}

void path_to_free(path_to *path) {
  free(path);
  path = NULL;
}

errors_of_path *errors_init() {
  errors_of_path *new = (errors_of_path *)malloc(sizeof(errors_of_path));
  if (new == NULL) {
    return NULL;
  }
  return new;
}

void errors_free(errors_of_path *error) {
  free(error);
  error = NULL;
}

char *black_symb(path_to *path) {
  char mass[] = {92, '*', '?', '<', '>', '|'};
  for (int i = 0; i < 6; i++) {
    char *check = sspn(path->string, mass[i]);
    if (check != NULL) {
      return check;
    }
  }
  return NULL;
}

void input(path_to *path) {
  printf("delim: +\n");
  printf("path: ");
  char b;
  while ((b = getchar()) != '\n') {
    path_to_add(path, b);
  }
}

void check(path_to *path, errors_of_path *error) {
  char *ck = sspn(path->string, '/');
  if (ck == NULL || (slen(path->string) < 3)) {
    error->type_of_error = ErrorTypeAintPath;
    return;
  }
  if (path->len > MAX_PATH) {
    error->type_of_error = ErrorTypeTooLongLenght;
    return;
  }
  char *check = black_symb(path);
  if (check != NULL) {
    error->type_of_error = ErrorTypeBannedSymbol;
    error->column = check;
    return;
  }
  for (int i = 0; i < path->len; i++) {
    if (!(path->string[i] >= 32 && path->string[i] <= 126)) {
      error->type_of_error = ErrorTypeVagueSymbol;
      error->column = &path->string[i];
      return;
    }
  }
  char *pointers_of_path[100];
  int count_of_path = stok(path->string, '+', pointers_of_path);
  for (int i = 0; i < count_of_path; i++) {
    char *pointers_of_dir[100];
    int count_of_dir = stok(pointers_of_path[i], '/', pointers_of_dir);
    int t = 0;
    if (slen(pointers_of_dir[t]) == 0) {
      t++;
    }
    if (scmp(pointers_of_dir[t], "cygdrive") == 0) {
      if (!((slen(pointers_of_dir[t + 1]) == 1) &&
            (*pointers_of_dir[t + 1] >= 97 &&
             *pointers_of_dir[t + 1] <= 122))) {
        error->column = pointers_of_dir[t + 1];
        error->type_of_error = ErrorTypeIncorrectPath;
        return;
      }
    }
    unstok(pointers_of_path[i], '/', pointers_of_dir, count_of_dir);
  }
  unstok(path->string, '+', pointers_of_path, count_of_path);
  error->type_of_error = ErrorTypeNon;
  return;
}

void process(path_to *path, errors_of_path *error, path_to *new) {
  if (error->type_of_error != 0) {
    return;
  }
  char *pointers_of_path[100];
  int count_of_path = stok(path->string, '+', pointers_of_path);
  for (int i = 0; i < count_of_path; i++) {
    int t = 0;
    char *pointers_of_dir[100];
    int count_of_dir = stok(pointers_of_path[i], '/', pointers_of_dir);
    if (slen(pointers_of_dir[0]) == 0) {
      t++;
    }
    if (scmp(pointers_of_dir[t], "cygdrive") == 0) {
      *pointers_of_dir[t + 1] = min_to_max(*pointers_of_dir[t + 1]);
      unstok(pointers_of_path[i], 92, pointers_of_dir, count_of_dir);
      path_to_add(new, *pointers_of_dir[t + 1]);
      path_to_add(new, ':');
      for (int k = 10 + t; k < slen(pointers_of_path[i]); k++) {
        path_to_add(new, pointers_of_path[i][k]);
      }
      if ((i + 1) < count_of_path) {
        path_to_add(new, '+');
      }
    } else {
      unstok(pointers_of_path[i], '/', pointers_of_dir, count_of_dir);
      for (int k = 0; k < slen(pointers_of_path[i]); k++) {
        path_to_add(new, pointers_of_path[i][k]);
      }
      if ((i + 1) < count_of_path) {
        path_to_add(new, '+');
      }
    }
  }
}

void output(path_to *path, errors_of_path *error, path_to *new) {
  switch (error->type_of_error) {
  case ErrorTypeNon:
  	if (scmp(path->string, new->string) == 0) {
  		printf("\n");
  		printf("Nothing to change\n");
  	}else {
  		printf("\n");
    	printf("new path: %s\n", new->string);
  	}
  	break;
  case ErrorTypeAintPath:
    printf("\n");
    printf("ERROR\n");
    printf("You didn't wrote a PATH!!!\n");
    break;
  case ErrorTypeTooLongLenght:
    printf("\n");
    printf("ERROR\n");
    printf("Your PATH is too long!!!\n");
    printf("Your size = %lu. Max size = 260 symbols\n", path->len);
    break;
  case ErrorTypeBannedSymbol:
    printf("\n");
    printf("ERROR\n");
    printf("Your PATH is contain symbol: '%c' from black-list!!!\n",
           *error->column);
    break;
  case ErrorTypeVagueSymbol:
    error->column[2] = '\0';
    printf("\n");
    printf("ERROR\n");
    printf("Your PATH is contain NOT available symbol: '%s'\n", error->column);
    break;
  case ErrorTypeIncorrectPath:
    printf("\n");
    printf("ERROR\n");
    printf("Incorrect PATH!!!\n");
    printf("Inaccuracy in the element :'%s'\n", error->column);
    break;
  }
}

int main() {
  path_to *path;
  path_to *new;
  errors_of_path *error;

  path = path_to_init();
  new = path_to_init();
  error = errors_init();

  if (path == NULL) {
    printf("Can't allocate memory\n");
    path_to_free(new);
    errors_free(error);
    return -1;
  }
  if (new == NULL) {
    printf("Can't allocate memory\n");
    path_to_free(path);
    errors_free(error);
    return -1;
  }
  if (error == NULL) {
    printf("Can't allocate memory\n");
    path_to_free(path);
    path_to_free(new);
    return -1;
  }

  input(path);
  check(path, error);
  process(path, error, new);
  output(path, error, new);

  path_to_free(path);
  path_to_free(new);
  errors_free(error);
}