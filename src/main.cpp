#include <iostream>
#include <fstream>
#include <filesystem>
#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"
#include "HyperPSearch.h"

int main(int argc, char* argv[])
{
    int end = 0;
    int vector_size;
    int num_units;
    while(end!=-1)
    {
        std::cout << "Flourite Germaninum Rush" << std::endl;
        // set things up
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
        int* best_vector = new int[vector_size];
        // Set your parameters
        int mode;
        std::cout << "Please enter the mode you want to use :" << std::endl;
        std::cout << "1. HyperPsearch" << std::endl;
        std::cout << "2. Default" << std::endl;
        std::cin >> mode;
        if (mode == 1)
        {
            int dice_num ;
            std::cout << "Please enter the number of dice you want to use :" << std::endl;
            std::cin >> dice_num;
            if(dice_num<=0){
                std::cout<<"Invalid dice, use 5 as default"<<std::endl;
                dice_num = 5;
            }
            randomSearch(dice_num,best_vector,vector_size);

        }
        else
        {

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
            std::cout << "To use the default value(0.90), please enter 0" << std::endl;
            std::cin >> parameters.crossover_rate;
            if (parameters.crossover_rate == 0)
            {
                parameters.crossover_rate = 0.9;
            }

            std::cout << "Please enter the generation size" << std::endl;
            std::cout << "To use the default value(1000), please enter 0" << std::endl;
            std::cin >> parameters.generation_step;
            if (parameters.generation_step == 0)
            {
                parameters.generation_step = 1000;
            }

            parameters.mutation_max_step = vector_size;
            
            std::cout << "Please enter the early stopping generations" << std::endl;
            std::cout << "To use the default value, please enter 0" << std::endl;
            std::cin >> parameters.early_stopping_generations;
            if (parameters.early_stopping_generations == 0)
            {
                parameters.early_stopping_generations = 100 + 2*(vector_size-1);
            }
            parameters.mutation_type_rate = { 0.8,0.2};
            parameters.crossover_type_rate = { 0.48,0.48,0.04 };

            parameters_.max_iterations = 1000;
            parameters_.tolerance = 0.1;

            double result = optimize(vector_size, best_vector, Evaluate_Circuit, parameters);
            
            // Generate final output
            std::cout << result << std::endl;
            std::cout << "                            " << std::endl;
            std::filesystem::path path = "..";
            path /= "..";
            path /= "Monetary_Value.txt";
            std::string file_name = path.string();
            std::fstream file;
            file.open(file_name, std::ios_base::out);
            if (file.is_open())
            {
                file << result;
            }
            else
            {
                std::cout << "Error when opening the file." << std::endl;
            }
            file.close();
        }
        std::filesystem::path path = "..";
        path /= "..";
        path /= "Circuit_Vector.txt";
        std::string file_name = path.string();
        std::fstream file;
        file.open(file_name, std::ios_base::out);
        if (file.is_open())
        {
            std::cout << "File opened successfully." << std::endl;
            for (int n = 0; n < vector_size; n++)
            {
                file << best_vector[n];
                if (n < vector_size - 1)
                {
                    file << " ";
                }
            }
        }
        else
        {
            std::cout << "Error when opening the file." << std::endl;
        }
        file.close();
        delete[] best_vector;
        std::cout<<"To continue using the program enter any number"<<std::endl;
        std::cout<<"To exit, please enter -1"<<std::endl;
        std::cin>>end;
    }
    return 0;

}
