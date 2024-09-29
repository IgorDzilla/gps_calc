#include "gps.h"
#include "auxillary.h"
#include "splitter.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_COORD 2

// this function may appear in other files as well. As it's easier to copy it
// than make a special file with it

/*
 * Expects string in format: dregrees:mins:secs
 *                              ^^^    ^^   ^^
 *                              int    int  double
 */
Angle *read_angle(char *string) {
  if (!string) {
    err_print(__FILE__, "read_angle()", "string is NULL");
    return NULL;
  }

  char **split = strsplit(string, ':');
  if (!split) {
    err_print(__FILE__, "read_angle()", "strsplit() function failed");
    return NULL;
  } else if (splitsize(split) != 3) {
    err_print(__FILE__, "read_angle()", "unexpected data format");
    return NULL;
  }

  Angle *angle = (Angle *)malloc(sizeof(Angle));
  if (!angle) {
    err_print(__FILE__, "read_angle()", "memory allocation failed");
    return NULL;
  }

  if (!sscanf(split[0], "%d", &angle->degrees) ||
      !sscanf(split[1], "%d", &angle->mins) ||
      !sscanf(split[2], "%Lf", &angle->secs)) {
    err_print(__FILE__, "read_angle()", "scanning values failed");
    free_split(split);
    free(angle);
    return NULL;
  }

  return angle;
}

Coord *read_coords(char *latitude, char *longtitude) {
  if (!longtitude || !latitude) {
    err_print(__FILE__, "read_coords", "one or both strings are NULL");
    return NULL;
  }

  Coord *coords = (Coord *)malloc(sizeof(Coord));
  if (!coords) {
    err_print(__FILE__, "read_coords()", "memory allocation failed");
    return NULL;
  }

  coords->longtitude = read_angle(longtitude);
  if (!coords->longtitude) {
    err_print(__FILE__, "read_coords()", "read_angle() function failed");
    free(coords);
    return NULL;
  }

  coords->latitude = read_angle(latitude);
  if (!coords->latitude) {
    err_print(__FILE__, "read_coords()", "read_angle() function failed");
    free(coords);
    return NULL;
  }

  return coords;
}

void free_coord(Coord *coord) {
  free(coord->latitude);
  free(coord->longtitude);
  free(coord);
}

int coord_arr_push_back(Coord ***coord_arr_ptr, size_t *size, Coord *new_coord);

Coord **read_file(char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    err_print(__FILE__, "read_file()", "opening file failed");
    return NULL;
  }

  // read the file into string
  fseek(file, 0, SEEK_END);
  unsigned long file_size = ftell(file);
  rewind(file);

  char buffer[file_size + 1];

  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  fclose(file);

  printf("FILE CONTENTS\n%s", buffer);
  // split the string
  char **splits = strsplit(buffer, '\n');
  if (!splits) {
    err_print(__FILE__, "read_file()", "strsplit() function failed");
    return NULL;
  }

  split_pop(&splits);

  Coord **coords_arr = NULL; // create holder of coords
  size_t size = 0;

  for (size_t i = 0; splits[i]; i++) {
    char **strings = strsplit(splits[i], ',');

    ///////////////
    for (size_t p = 0; strings[p]; p++)
      printf("->%s\n", strings[p]);
    /////////////
    // self termination in case of failure
    if (!strings) {
      err_print(__FILE__, "read_file()", "strsplit() funciton failed");
      // clean up
      for (size_t idx = 0; idx < size; idx++)
        free_coord(coords_arr[i]);
      free_split(splits);
      free_split(strings);
      return NULL;
    }

    size_t s;
    if ((s = splitsize(strings)) != SIZE_OF_COORD) {
      char help_str[16];
      err_print(__FILE__, "read_file()",
                strcat("incorrect data. Should be two angles, got",
                       sprintf(help_str, "%zu", s)));
      return NULL;
    }

    printf("FINE 1\n");
    Coord *new_coord = read_coords(strings[0], strings[1]);
    if (!new_coord) {
      err_print(__FILE__, "read_file()", "read_coords() function failed");
      // in case of failure clean up
      for (size_t idx = 0; idx < size; idx++)
        free_coord(coords_arr[i]);
      free(coords_arr);
      free_split(splits);
      free_split(strings);
    }

    printf("LATITUDE: %d:%d:%Lf\t", new_coord->latitude->degrees,
           new_coord->latitude->mins, new_coord->latitude->secs);
    printf("LONGTITUDE: %d:%d:%Lf\n", new_coord->longtitude->degrees,
           new_coord->longtitude->mins, new_coord->longtitude->secs);

    printf("FINE 2\n");
    if (!coord_arr_push_back(&coords_arr, &size, new_coord)) {
      err_print(__FILE__, "read_file()",
                "coord_arr_push_back() function failed");
      for (size_t idx = 0; idx < size; idx++)
        free_coord(coords_arr[idx]);
      free(coords_arr);
      free_split(splits);
      free_split(strings);
    }

    free_split(strings);
  }

  free(coords_arr);

  printf("FINE 3\n");
  if (!coord_arr_push_back(&coords_arr, &size, NULL)) {
    err_print(__FILE__, "read_file()", "coord_arr_push_back() function failed");
    for (size_t idx = 0; idx < size; idx++)
      free_coord(coords_arr[idx]);
    return NULL;
  }

  return coords_arr;
}

int coord_arr_push_back(Coord ***coord_arr_ptr, size_t *size,
                        Coord *new_coord) {
  Coord **new_coords_arr =
      (Coord **)realloc(*coord_arr_ptr, sizeof(Coord *) * (*size + 1));
  if (!new_coords_arr) {
    err_print(__FILE__, "coord_arr_push_back()", "memory reallocation failed");
    return 0; //-112654365171829;
  }

  new_coords_arr[*size++] = new_coord;
  *coord_arr_ptr = new_coords_arr;

  return 1;
}
