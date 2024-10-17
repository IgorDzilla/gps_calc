#include "auxillary.h"
#include "gps.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int help(void);
int run(const char *filename);

int main(int argc, char *argv[]) {
  switch (argc) {
  case 1:
    help();
    return 0;
  case 2:
    return run(argv[1]);
  default:
    fprintf(stderr, "Uknown cli arguemnts. Run \"gps_calc\" for help\n");
    return 1;
  }
}

int help(void) {
  printf("gps_calc - small app for calculating distance between two "
         "geographical coordinates. Uses Vincenty formula for computing.\n");
  printf("Usage: gps_calc [filename]\n");
  printf("Data is passed in *.gcrd files\n");
  printf("File sample:\n");
  printf("\t59:57:0,30:19:0\n\t55:44:24.00,37:36:36.00\n");
  printf("\nNOTE: This program was made for studying purposes. Please, take "
         "this into consideration\n");

  return 0;
}

void print_coord(Coord *coord) {
  printf("\t\t%s\t%d:%d:%Lf\n", "latitude", coord->latitude->degrees,
         coord->latitude->mins, coord->latitude->secs);
  printf("\t\t%s\t%d:%d:%Lf\n", "longtiude", coord->longtitude->degrees,
         coord->longtitude->mins, coord->longtitude->secs);
}

int run(const char *filename) {
  printf("===STARTING COMPUTING===\n\nEARTH PARAMETERS:\n");
  EarthParams earth = EARTH_PARAMS;
  printf("\tALFAZ\t%.6Lf meters\n", earth.alfaz);
  printf("\tAXA\t%.6Lf meters\n", earth.axa);
  printf("\tAXB\t%.6Lf meters\n\n\n", earth.axb);

  Coord **coords = NULL;
  int coords_size = read_file(filename, &coords);

  if (!coords_size) {
    err_print(__FILE__, "main()", "read_file() function failed");
    return 1;
  }

  printf("COORDINATES\n");
  for (int i = 0; i < coords_size; i++) {
    printf("\tPoint %d\n", i + 1);
    print_coord(coords[i]);
  }

  long double distance = vincenty(coords[0], coords[1]);
  printf("\nCOMPUTING RESULT\n");
  printf("\tDistance: %.5Lf meters or %.5Lf km\n", distance, distance / 1000);

  printf("\n===END OF COMPUTING===\n");

  return 0;
}
