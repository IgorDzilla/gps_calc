#include "gps.h"
#include "splitter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void err_print(char *src, char *msg) {
  fprintf(stderr, "ERROR\t%s\n", msg);
  fprintf(stderr, "MESSAGE\t%s\n", msg);
}

/*
 * Expects string in format: dregrees:mins:secs
 *                              ^^^    ^^   ^^
 *                              int    int  double
 */
Angle *read_angle(char *string) {
  if (!string) {
    return NULL;
  }
}
