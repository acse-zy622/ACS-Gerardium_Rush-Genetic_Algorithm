#include <iostream>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"
#include <chrono>
int main(int argc, char* argv[])
{

    // set things up
    int vector_size = 11;

    // Set your parameters
    Circuit_Parameters parameters_;
    Algorithm_Parameters parameters;

    parameters.population_size = 100;
    parameters.mutation_rate = 0.05;
    parameters.crossover_rate = 0.85;
    parameters.generation_size = 75;
    parameters.mutation_max_step = 6;
    parameters.early_stopping_generations = 25;
    parameters.mutation_type_rate = {0.33,0.67};
    parameters.crossover_type_rate = {0.33,0.33,0.34};

    parameters_.max_iterations = 1000;
    parameters_.tolerance = 0.01;


    //Individual I;
    //I.vector = { 0, 1, 2, 3, 0, 0, 4 };
    //int num_units = 3; // Initialize with appropriate value
    //Circuit circuit(num_units);

    //if (circuit.Check_Validity(I.vector.data())) {
    //    std::cout << "The circuit is valid.\n";
    //}
    //else {
    //    std::cout << "The circuit is not valid.\n";
    //}
    //23250041006
    // run your code
    auto total_start = std::chrono::high_resolution_clock::now();
    double result = optimize(vector_size,Evaluate_Circuit,parameters);
    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_elapsed = total_end - total_start;
    std::cout << result << std::endl;
    std::cout << "Total running time: " << total_elapsed.count() << " seconds" << std::endl;

    return 0;
}
