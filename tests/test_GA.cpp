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
    int num_of_unit = 2 * vector_size - 1;

    // set your parameters
    Circuit_Parameters parameters_;
    Algorithm_Parameters parameters;

    //[100,150]
    parameters.population_size = 10;
    // bounded with mutation_max_step;
    //[0.01,0.05]
    parameters.mutation_rate = 0.05;
    //[0.8,0.95]
    parameters.crossover_rate = 0.90;
    parameters.generation_step = 10;
    // has somthing to do with the 2*n + 1;
    parameters.mutation_max_step = vector_size;

    parameters.early_stopping_generations = 10;

    //[first one takes the majority]
    parameters.mutation_type_rate = {0.8,0.2};

    //last one takes the smallest.
    parameters.crossover_type_rate = {0.48,0.48,0.04};


    parameters_.max_iterations = 1000;
    parameters_.tolerance = 0.01;
    int* vector = new int[vector_size];
    // unit_test and 23250041006 is the unit case
    auto total_start_ = std::chrono::high_resolution_clock::now();
    double result_ = optimize(vector_size,vector,Evaluate_circuit,parameters);
    auto total_end_ = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_elapsed_ = total_end_ - total_start_;
    std::cout << result_ << std::endl;
    std::cout << "Total running time: " << total_elapsed_.count() << " seconds" << std::endl;
    std::cout << "                      " << std::endl;

    // // run your real code here
    // auto total_start = std::chrono::high_resolution_clock::now();
    // double result = optimize(vector_size, Evaluate_Circuit, parameters);
    // auto total_end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> total_elapsed = total_end - total_start;
    // std::cout << result << std::endl;
    // std::cout << "Total running time: " << total_elapsed.count() << " seconds" << std::endl;

    return 0;
}

