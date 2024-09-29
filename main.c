#include "gps.h"
#include <stdio.h>
#include <stdlib.h>

void print(Coord *coord) {
  printf("LATITUDE: %d:%d:%Lf\t", coord->latitude->degrees,
         coord->latitude->mins, coord->latitude->secs);
  printf("LONGTITUDE: %d:%d:%Lf\n", coord->longtitude->degrees,
         coord->longtitude->mins, coord->longtitude->secs);
}

int main() {
  EarthParams earth = EARTH_PARAMS;
  printf("ALFAZ\t%.15Lf\n", earth.alfaz);
  printf("AXA\t%.15Lf\n", earth.axa);
  printf("AXB\t%.15Lf\n", earth.axb);

  char *input_string = "37:12:34.567";
  Angle *angle = read_angle(input_string);

  if (angle) {
    printf("Degrees: %d\n", angle->degrees);
    printf("Minutes: %d\n", angle->mins);
    printf("Seconds: %.3Lf\n", angle->secs);
    free(angle);
  }

  Coord **data = read_file("data.gcrd");

  for (size_t i = 0; data[i]; i++) {
    print(data[i]);
  }

  return 0;
}
