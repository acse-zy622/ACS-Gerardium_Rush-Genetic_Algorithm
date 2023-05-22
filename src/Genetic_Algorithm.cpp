#include <stdio.h>
#include <cmath>
#include <array>

#include "Genetic_Algorithm.h"

int optimize(int vector_size, int * vector, double (&func) (int, int*),
                           struct Algorithm_Parameters parameters) {

  /**
     This function optimizes an input vector with repect to the function passed to it via a genetic algorithm approach.
     The function passed to it should return a double that
     represents the performance of the vector.  The optimize function updates the vector with
     the best solution found.

     The function should return non-zero if it fails to find an optimal solution, the precise
     value return can have meaning.

  */

  // Run the genetic algorithm process

  // Update the vector with the best solution found

  return 0;

}

struct Algorithm_Parameters default_algorithm_parameters = {1000};

int optimize(int vector_size,
                int * vector,
                double (&func) (int, int*))
                {return optimize(vector_size, vector,
                                 func, default_algorithm_parameters);};

// additional variables, classes and functions as needed.