#include "gps.h"
#include <stdio.h>

int main() {
  EarthParams earth = EARTH_PARAMS;
  printf("ALFAZ\t%.15Lf\n", earth.alfaz);
  printf("AXA\t%.15Lf\n", earth.axa);
  printf("AXB\t%.15Lf\n", earth.axb);

  return 0;
}
