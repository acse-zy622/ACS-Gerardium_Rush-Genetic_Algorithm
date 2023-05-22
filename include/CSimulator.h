/** header file for the circuit simulator
 * 
 * This header file defines the function that will be used to evaluate the circuit
*/

#pragma once

struct Circuit_Parameters{
    double tolerance;
    int max_iterations;
    // other parameters for your circuit simulator       
};

double Evaluate_Circuit(int vector_size, int *circuit_vector, struct Circuit_Parameters parameters);
double Evaluate_Circuit(int vector_size, int *circuit_vector);
