#include "auxillary.h"

#include <stdio.h>

void err_print(char *file_name, char *src, char *msg) {
  fprintf(stderr, "SOURCE\t%s - %s\n", file_name, src);
  fprintf(stderr, "ERROR\t%s\n", msg);
}
