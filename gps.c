#include "gps.h"
#include "auxillary.h"
#include "splitter.h"

#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_COORD 2

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
void free_coord_arr(Coord **arr, size_t size);

int read_file(const char *file_name, Coord ***arr) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    err_print(__FILE__, "read_file()", "opening file failed");
    return -1;
  }

  // read the file into string
  fseek(file, 0, SEEK_END);
  unsigned long file_size = ftell(file);
  rewind(file);

  char buffer[file_size + 1];

  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  fclose(file);

  // split the string
  char **lines = strsplit(buffer, '\n');
  if (!lines) {
    err_print(__FILE__, "read_file()", "strsplit() function failed");
    return -1;
  }

  split_pop(&lines);

  size_t size = 0;

  for (size_t i = 0; lines[i]; i++) {
    char **strings = strsplit(lines[i], ',');

    // self termination in case of failure
    if (!strings) {
      err_print(__FILE__, "read_file()", "strsplit() funciton failed");
      // clean up
      for (size_t idx = 0; idx < size; idx++)
        free_coord(*arr[i]);
      free_split(lines);
      free_split(strings);
      return -1;
    }

    size_t s;
    if ((s = splitsize(strings)) != SIZE_OF_COORD) {
      char help_str[16];
      err_print(__FILE__, "read_file()",
                strcat("incorrect data. Should be two angles, got",
                       sprintf(help_str, "%zu", s)));
      return -1;
    }

    Coord *new_coord = read_coords(strings[0], strings[1]);
    if (!new_coord) {
      err_print(__FILE__, "read_file()", "read_coords() function failed");
      // in case of failure clean up
      for (size_t idx = 0; idx < size; idx++)
        free_coord(*arr[i]);
      free_split(lines);
      free_split(strings);
      return -1;
    }

    if (coord_arr_push_back(arr, &size, new_coord)) {
      err_print(__FILE__, "read_file()",
                "coord_arr_push_back() function failed");
      for (size_t idx = 0; idx < size; idx++)
        free_coord(*arr[idx]);
      free_split(lines);
      free_split(strings);
      return 0;
    }

    free_split(strings);
  }

  free_split(lines);

  return size;
}

int coord_arr_push_back(Coord ***coord_arr_ptr, size_t *size,
                        Coord *new_coord) {
  Coord **new_coords_arr =
      (Coord **)realloc(*coord_arr_ptr, sizeof(Coord *) * (*size + 1));
  if (!new_coords_arr) {
    err_print(__FILE__, "coord_arr_push_back()", "memory reallocation failed");
    return ENOMEM; //-112654365171829;
  }

  new_coords_arr[*size] = new_coord;
  *size += 1;
  *coord_arr_ptr = new_coords_arr;

  return 0;
}

long double convert_to_degrees(Angle *angle) {
  long double result = (long double)angle->degrees;

  // convert minutes to degrees
  result += (long double)angle->mins / 60.0;

  // convert seconds to degrees
  result += angle->secs / 3600.0;

  return result;
}

long double degrees2rads(long double degrees) { return degrees * M_PI / 180; }

void free_coord_arr(Coord **arr, size_t size) {
  for (size_t i = 0; i < size; i++)
    free_coord(arr[i]);
  free(arr);
}

long double vincenty(Coord *point1, Coord *point2) {
  long double a = AXA;
  long double f = ALFAZ;
  double b = AXB;

  // variable initilization
  long double u1, u2, lon, lambda = 0., precision, sin_sigma, cos_sigma, sigma,
                           sin_alpha, cos_sq_alpha, cos2sigma;
  long double A, B, C, lambda_precise, delta_sig, distance, usq;

  // convert to radians
  long double lat1 = degrees2rads(convert_to_degrees(point1->latitude));
  long double long1 = degrees2rads(convert_to_degrees(point1->longtitude));
  long double lat2 = degrees2rads(convert_to_degrees(point2->latitude));
  long double long2 = degrees2rads(convert_to_degrees(point2->longtitude));

  u1 = atan((1 - f) * tan(lat2));
  u2 = atan((1 - f) * tan(lat1));

  lon = long1 - long2;
  lambda = lon;
  precision = 10e-12; // pow(10., -12.); // iteration tolerance

  while (fabsl(lambda_precise - lambda) >= precision) {
    sin_sigma =
        sqrt(pow((cos(u2) * sin(lambda)), 2.) +
             pow(cos(u1) * sin(u2) - sin(u1) * cos(u2) * cos(lambda), 2.));
    cos_sigma = sin(u1) * sin(u2) + cos(u1) * cos(u2) * cos(lambda);
    sigma = atan(sin_sigma / cos_sigma);
    sin_alpha = (cos(u1) * cos(u2) * sin(lambda)) / sin_sigma;
    cos_sq_alpha = 1 - pow(sin_alpha, 2.);
    cos2sigma = cos_sigma - ((2 * sin(u1) * sin(u2)) / cos_sq_alpha);
    C = (f / 16) * cos_sq_alpha * (4 + f * (4 - 3 * cos_sq_alpha));
    lambda_precise = lambda;
    lambda =
        lon + (1 - C) * f * sin_alpha *
                  (sigma + C * sin_sigma *
                               (cos2sigma +
                                C * cos_sigma * (2 * pow(cos2sigma, 2.) - 1)));
  }

  usq = cos_sq_alpha * ((pow(a, 2.) - pow(b, 2.)) / pow(b, 2.));
  A = 1 + (usq / 16384) * (4096 + usq * (-768 + usq * (320 - 175 * usq)));
  B = (usq / 1024) * (256 + usq * (-128 + usq * (74 - 47 * usq)));
  delta_sig = B * sin_sigma *
              (cos2sigma +
               0.25 * B *
                   (cos_sigma * (-1 + 2 * pow(cos2sigma, 2.)) -
                    (1.0L / 6) * B * cos2sigma * (-3 + 4 * pow(sin_sigma, 2.)) *
                        (-3 + 4 * pow(cos2sigma, 2.))));
  distance = b * A * (sigma - delta_sig);

  return distance;
}
