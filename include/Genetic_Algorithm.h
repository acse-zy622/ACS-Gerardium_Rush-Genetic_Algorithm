#ifndef GA_H
#define GA_H

#include <vector>

struct Algorithm_Parameters {
    int population_size;
    double mutation_rate;
    double crossover_rate;
    int generation_size;
    int mutation_max_step;
    int early_stopping_generations;
    std::vector<double> mutation_type_rate;
    std::vector<double> crossover_type_rate;
};

struct Individual {
    std::vector<int> vector;
    double fitness_val;
};

// Function types
//double Evaluate_Circuit(int vector_size, int* circuit_vector);
double Evaluate_circuit(int vector_size, int* circuit_vector);

int optimize(int vector_size, double(&func) (int, int*), Algorithm_Parameters parameters);


#endif // GA_H