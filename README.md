# GPS Calculator

This project is a GPS calculator that reads coordinates from a file and calculates the distance between two points using the Vincenty formula. The coordinates are stored in a struct called `Coord` and an auxiliary struct called `Angle`. The main functions of the project are:

1. `read_file(const char *file_name, Coord ***arr)`: This function reads the coordinates from a file and stores them in an array of pointers to `Coord` structs. It returns the size of the array.

2. `vincenty(Coord *point1, Coord *point2)`: This function calculates the distance between two points using the Vincenty formula. It takes two `Coord` structs as input and returns the distance in kilometers.

The project also includes helper functions for reading and splitting strings, allocating and freeing memory, and converting angles to degrees.

## Getting Started

To get started with this project, follow these steps:

1. Clone the repository to your local machine:

   ```
   https://github.com/IgorDzilla/gps_calc/new/master?filename=README.md
   ```

2. Navigate to the project directory:

   ```
   cd gps_calc
   ```

3. Compile the project using make:

   ```
   make
   ```

4. Run the compiled program with a file containing coordinates:

   ```
   ./gps_calc coords.gcrd
   ```
