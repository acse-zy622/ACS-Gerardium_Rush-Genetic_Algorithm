#ifndef GA_H
#define GA_H
#include <stdio.h>
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include "CCircuit.h"

struct Algorithm_Parameters {
    int population_size;
    double mutation_rate;
    double crossover_rate;
    int generation_step;
    int mutation_max_step;
    int early_stopping_generations;
    std::vector<double> mutation_type_rate;
    std::vector<double> crossover_type_rate;
};

struct Individual {
    std::vector<int> vector;
    double fitness_val;
};

double Evaluate_circuit(int vector_size, int* circuit_vector);

std::vector<Individual> InitializePopulation(int vector_size, std::mt19937& gen, std::uniform_int_distribution<>& uniform_int, CCircuit& circuit, Algorithm_Parameters& parameters);

void CalculateFitness(std::vector<Individual>& parents, double(&func) (int, int*));

void SortParentsByFitness(std::vector<Individual>& parents);

std::unique_ptr<std::vector<double>> NormalizeFitness(std::vector<Individual>& parents);

std::unique_ptr<std::vector<double>> MakeCDF(std::unique_ptr<std::vector<double>>& normalized);

std::pair<Individual, Individual> SelectParents(std::vector<Individual>& parents, std::unique_ptr<std::vector<double>>& cdf, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real);

std::pair<Individual, Individual> Crossover(Individual& selected_parentX, Individual& selected_parentY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, Algorithm_Parameters& parameters, int vector_size);

void Mutation(Individual& childX, Individual& childY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, std::uniform_int_distribution<>& uniform_int, std::uniform_real_distribution<>& uniform_mutation_step, Algorithm_Parameters& parameters, int vector_size);

void AddChildren(std::vector<Individual>& children, Individual& childX, Individual& childY, CCircuit& circuit);

double optimize(int vector_size, double(&func) (int, int*), Algorithm_Parameters parameters);


#endif 