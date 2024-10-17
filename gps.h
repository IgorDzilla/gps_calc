#ifndef GPS_H
#define GPS_H

#include <stdlib.h>

typedef struct _Angle {
  int degrees;
  int mins;
  long double secs;
} Angle;

typedef struct _Coords {
  Angle *longtitude;
  Angle *latitude;
} Coord;

#define ALFAZ (1.0L / 298.257223563)
#define AXA 6378137.0L // meters
#define AXB (AXA * (1.0L - ALFAZ))

typedef struct _EarthParams {
  long double alfaz;
  long double axa;
  long double axb;
} EarthParams;

#define EARTH_PARAMS {ALFAZ, AXA, AXB}

/////////////////////////////////////////
/// FUNCTION DECLARATIONS
/////////////////////////////////////////

/* Reads angle in format 'dregrees:mins:secs' to struct Angle
 * Return value: pointer to Angle struct or NULL if function failed
 * NOTE: this function is mainly auxiallary
 */
Angle *read_angle(char *string);

/* Creates a Coords object from to angles in string format
 * Return value: pointer to Angle struct or NULL if function failed
 */
Coord *read_coords(char *latitude, char *longtitude);

/* Fress all the memory allocated for Coord including pointer itself */
void free_coord(Coord *coord);

/* Fress all the memory allocated for an array of Coord elements including
 * pointer itself */
void free_coord_arr(Coord **arr, size_t size);

/*
 * Read data from *.gcrd files into array of Coord's
 * Parameters: file_name - name of file
 *             arr_ptr - pointer to the array of coords
 * Return value: success - size of arr_ptr
 *               error - -1
 */
int read_file(const char *file_name, Coord ***arr_ptr);

/*Calculates distance between 2 coords*/
long double distance(Coord *point1, Coord *point2);

/*Converts degrees value into radians*/
long double degrees2rads(long double degrees);

long double convert_to_degrees(Angle *angle);

long double vincenty(Coord *point1, Coord *point2);

#endif // GPS_H
