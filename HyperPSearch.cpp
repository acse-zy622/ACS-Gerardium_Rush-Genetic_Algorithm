#include <iostream>
#include <random>
#include <vector>
#include "Genetic_Algorithm.h"
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"
#include "Genetic_Algorithm.h"

Algorithm_Parameters generateRandomParameters(int vector_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> populationRander(100, 150);
    std::uniform_real_distribution<> mutationRander(0.01, 0.05);    
    std::uniform_real_distribution<> crossoverRander(0.8, 0.95);
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
    params.generation_step = 500;
    params.mutation_max_step = vector_size;
    params.early_stopping_generations = 100 + 1.2 * vector_size;

    return params;
}

void randomSearch(int iterCount, int vector_size) {
    double bestScore = -1.0;
    int bestPopulation = 0;
    double bestMutationR = 0.0;
    double bestCrossoverR = 0.0;
    std::vector<double> bestMTR;
    std::vector<double> bestCTR;
    for (int i = 0; i < iterCount; ++i) {
        Algorithm_Parameters customizePara = generateRandomParameters(vector_size);
        double temresult = 0.0;
        double result = 0.0;
        for (int o = 0; o < 3; ++o) {
            temresult += optimize(vector_size, Evaluate_Circuit, customizePara);
        }
        result = temresult / 3;
        std::cout << "Tem score for iter " << i <<":" <<result<< "\n";
        if (result > bestScore) {
            bestScore = result;
            bestPopulation = customizePara.population_size;
            bestMutationR = customizePara.mutation_rate;
            bestCrossoverR = customizePara.crossover_rate;
            bestMTR = customizePara.mutation_type_rate;
            bestCTR = customizePara.crossover_type_rate;
        }
    }    
    std::cout << "Best score: " << bestScore << "\n";
    std::cout << "Best parameters:\n";
    std::cout << "  Population size: " << bestPopulation << "\n";    
    std::cout << "  Mutation rate: " << bestMutationR << "\n";
    std::cout << "  Crossover rate: " << bestCrossoverR << "\n";    
    std::cout << "  mutation_type_rate rate: " << bestMTR[0] << bestMTR[1] << "\n";
    std::cout << "  crossover_type_rate rate: " << bestCTR[0] << bestCTR[1] << bestCTR[2] << "\n";
}