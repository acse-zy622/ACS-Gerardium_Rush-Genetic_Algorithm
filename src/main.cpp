#include <iostream>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"

int main(int argc, char* argv[])
{
    int end = 0;
    while(end!=-1)
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

    std::cout << "Please enter the population size" << std::endl;
    std::cout << "To use the default value(100), please enter 0" << std::endl;
    std::cin >> parameters.population_size;
    if (parameters.population_size == 0)
    {
        parameters.population_size = 100;
    }

    std::cout << "Please enter the mutation rate" << std::endl;
    std::cout << "To use the default value(0.05), please enter 0" << std::endl;
    std::cin >> parameters.mutation_rate;
    if (parameters.mutation_rate == 0)
    {
        parameters.mutation_rate = 0.05;
    }

    std::cout << "Please enter the crossover rate" << std::endl;
    std::cout << "To use the default value(0.85), please enter 0" << std::endl;
    std::cin >> parameters.mutation_rate;
    if (parameters.crossover_rate == 0)
    {
        parameters.crossover_rate = 0.85;
    }

    std::cout << "Please enter the generation size" << std::endl;
    std::cout << "To use the default value(500), please enter 0" << std::endl;
    std::cin >> parameters.generation_step;
    if (parameters.generation_step == 0)
    {
        parameters.generation_step = 500;
    }

    parameters.mutation_max_step = 5;
    
    std::cout << "Please enter the early stopping generations" << std::endl;
    std::cout << "To use the default value(500), please enter 0" << std::endl;
    std::cin >> parameters.early_stopping_generations;
    if (parameters.early_stopping_generations == 0)
    {
        parameters.early_stopping_generations = 500;
    }
    parameters.mutation_type_rate = { 0.33,0.33,0.34 };
    parameters.crossover_type_rate = { 0.33,0.33,0.34 };

    parameters_.max_iterations = 1000;
    parameters_.tolerance = 0.1;

    double result = optimize(vector_size, Evaluate_Circuit, parameters);

    // Generate final output
    std::cout << result << std::endl;

    std::cout<<"To continue using the program enter any number"<<std::endl;
    std::cout<<"To exit, please enter -1"<<std::endl;
    std::cin>>end;
    }
    return 0;

}
