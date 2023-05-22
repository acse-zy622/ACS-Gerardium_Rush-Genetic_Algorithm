#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"

#include <cmath>

struct Circuit_Parameters default_circuit_parameters = {0.01, 1000};

int dummy_answer_vector[] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9,
				10, 11, 10, 11, 10, 11, 10, 11};

double Evaluate_Circuit(int vector_size, int* circuit_vector, struct Circuit_Parameters parameters) {
//This function takes a circuit vector and returns a performance value.
//The current version of the function is a dummy function that returns
// a performance value based on how close the circuit vector is to a predetermined answer vector.

  double Performance = 0.0; 
  for (int i=0;i<vector_size;i++) {
    //dummy_answer_vector is a predetermined answer vector (same size as circuit_vector)
    Performance += (20-std::abs(circuit_vector[i]-dummy_answer_vector[i]))*100.0; 
  }
  return Performance; 
}

// overloads
double Evaluate_Circuit(int vector_size, int* circuit_vector){
    return Evaluate_Circuit(vector_size, circuit_vector, default_circuit_parameters);
};

// Other functions and variables to evaluate a real circuit.