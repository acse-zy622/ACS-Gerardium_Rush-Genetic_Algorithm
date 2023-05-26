#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <filesystem>
#include "Genetic_Algorithm.h"
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"
#include "Genetic_Algorithm.h"
#include "HyperPSearch.h"

/**
 * @brief Generates algorithm parameters randomly.
 *
 * This function generates a set of genetic algorithm parameters (both random and non-random).
 * Random parameters: population size, mutation rate, crossover rate, mutation type rate, crossover type rate.
 * Non-random parameters: generation step, mutation max step, and early stopping generations.
 *
 * @param vector_size Genetic algorithm vector size for mutation max step and early stopping calculation.
 * @return Algorithm_Parameters The randomly generated parameters.
 */
Algorithm_Parameters generateRandomParameters(int vector_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> populationRander(100, 125);
    std::uniform_real_distribution<> mutationRander(0.01, 0.05);    
    std::uniform_real_distribution<> crossoverRander(0.85, 0.95);
    std::uniform_real_distribution<> mtRander(0.5, 0.9);
    std::uniform_real_distribution<> ctRander(0.25, 0.49);

    Algorithm_Parameters params;
    params.population_size = populationRander(gen);
    params.mutation_rate = mutationRander(gen);
    params.crossover_rate = crossoverRander(gen);
    double mtA = mtRander(gen);
    params.mutation_type_rate = {mtA, 1.0 - mtA};
    double ctA = ctRander(gen);
    double ctB = ctA;
    params.crossover_type_rate = {ctA, ctB, 1.0 - ctA - ctB};
    params.generation_step = 1000;
    params.mutation_max_step = vector_size;
    params.early_stopping_generations = 100 + 2 * (vector_size-1);

    return params;
}

/**
 * @brief Performs random search to find the best hyperparameters for genetic algorithm.
 *
 * This function performs a search by iteratively (and randomly) generating algorithm parameters,
 * and evaluating the genetic algorithm output with those parameters. 
 * At the end, it outputs the best score, best hyperparameters, 
 * and best vector to the console and writes the best score to a file.
 *
 * @param iterCount The number of iterations to perform.
 * @param best_vector A pointer to an array that will hold the best vector.
 * @param vector_size Genetic algorithm vector size.
 */
void randomSearch(int iterCount, int* best_vector, int vector_size) {
    double bestScore = -1.0;
    int bestPopulation = 0;
    double bestMutationR = 0.0;
    double bestCrossoverR = 0.0;
    std::vector<double> bestMTR;
    std::vector<double> bestCTR;
    for (int i = 0; i < iterCount; ++i) {
        Algorithm_Parameters customizePara = generateRandomParameters(vector_size);
        double* temresult = new double[3];
        double result = 0.0;
        int** tmp_best_vector = new int*[3];
        for (int o = 0; o < 3; ++o) {
            tmp_best_vector[o] = new int[vector_size];
            temresult[o] = optimize(vector_size, tmp_best_vector[o], Evaluate_Circuit, customizePara);
        }
        int max_index;
        max_index = std::max_element(temresult, temresult + 3) - temresult;
        result = temresult[max_index];
        std::cout << "Tem score for iter " << i <<":" <<result<< "\n";
        if (result > bestScore) {
            bestScore = result;
            for (int j = 0; j < vector_size; ++j) {
                best_vector[j] = tmp_best_vector[max_index][j];
            }
            bestPopulation = customizePara.population_size;
            bestMutationR = customizePara.mutation_rate;
            bestCrossoverR = customizePara.crossover_rate;
            bestMTR = customizePara.mutation_type_rate;
            bestCTR = customizePara.crossover_type_rate;
        }
    } 
    std::cout << "                            " << std::endl;
    std::filesystem::path path = "..";
    path /= "..";
    path /= "Monetary_Value.txt";
    std::string file_name = path.string();

    std::fstream file;
    file.open(file_name, std::ios_base::out);
    if (file.is_open())
    {
        file << bestScore;
    }
    else
    {
        std::cout << "Error when opening the file." << std::endl;
    }
    file.close();


    file << "Best score: " << bestScore << "\n";
    file.close();

    std::cout << "                            " << std::endl;  
    std::cout << "                            " << std::endl; 
    std::cout << "THE RESULT OF SEARCHING IS: \n";
    std::cout << "Best score: " << bestScore << "\n";
    std::cout << "Best vector: ";
    for (int i = 0; i < vector_size; ++i) {
        std::cout << best_vector[i] << " ";
    }
    std::cout << "\n";
    std::cout << "Best parameters are: \n";
    std::cout << "Population size: " << bestPopulation << "\n";    
    std::cout << "Mutation rate: " << bestMutationR << "\n";
    std::cout << "Crossover rate: " << bestCrossoverR << "\n";    
    std::cout << "Mutation_type_rates are " <<"Substitutions:  " << bestMTR[0] << " Rearrangements: "<< bestMTR[1] << "\n";
    std::cout << "Crossover_type_rates are " <<"One-point: "<< bestCTR[0] <<" Multiple-points: "<< bestCTR[1] <<" Uniform: "<< bestCTR[2] << "\n";
}
