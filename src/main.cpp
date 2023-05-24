#include <iostream>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"

int main(int argc, char* argv[])
{
    std::cout << "Flourite Germaninum Rush" << std::endl;
    // set things up
    int vector_size;
    int num_units;
    std::cout << "Please enter the number of units for the circuit" << std::endl;
    std::cout << "To use the default value(5 units), please enter 0" << std::endl;
    std::cin >> num_units;
    if (num_units == 0)
    {
        vector_size = 11;
    }
    else {
        vector_size = (2 * num_units) + 1;
    }
    // Set your parameters
    Circuit_Parameters parameters_;
    Algorithm_Parameters parameters;

    parameters.population_size = 100;
    parameters.mutation_rate = 0.05;
    parameters.crossover_rate = 0.85;
    parameters.generation_size = 500;
    parameters.mutation_max_step = 5;
    parameters.early_stopping_generations = 500;
    parameters.mutation_type_rate = { 0.33,0.33,0.34 };
    parameters.crossover_type_rate = { 0.33,0.33,0.34 };

    parameters_.max_iterations = 1000;
    parameters_.tolerance = 0.1;

    double result = optimize(vector_size, Evaluate_Circuit, parameters);

    // Generate final output
    std::cout << result << std::endl;

    return 0;
}
