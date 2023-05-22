/** Header for the Genetic Algorithm library
 *
*/

#pragma once

struct Algorithm_Parameters{
    int max_iterations;
    // other parameters for your algorithm       
};

int optimize(int, int *,
                double (&) (int, int*),
                struct Algorithm_Parameters);

// Other overloaded functions for optimize

int optimize(int, int *, double (&) (int, int*));

// Other functions and variables