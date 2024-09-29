#ifndef GPS_H
#define GPS_H

typedef struct _Angle {
  int degrees;
  int mins;
  long double secs;
} Angle;

typedef struct _Coords {
  Angle *longtitude;
  Angle *latitude;
} Coord;

#define ALFAZ 0.0033528244L
#define AXA 637816.0L // meters
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

void free_coord(Coord *coord);

Coord **read_file(char *file_name);

long double distance(Coord *point1, Coord *point2);

#endif // GPS_H
