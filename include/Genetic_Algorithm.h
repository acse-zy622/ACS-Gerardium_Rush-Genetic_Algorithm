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
#include <memory>
#include "CCircuit.h"
/**
 * @struct Algorithm_Parameters
 * @brief Contains parameters used to control the behavior of the genetic algorithm.
 *
 * @var Algorithm_Parameters::population_size
 * Size of the population in each generation.
 *
 * @var Algorithm_Parameters::mutation_rate
 * The chance that a mutation will occur during reproduction.
 *
 * @var Algorithm_Parameters::crossover_rate
 * The chance that a crossover will occur during reproduction.
 *
 * @var Algorithm_Parameters::generation_step
 * The maximum number of generations for which the algorithm should run.
 *
 * @var Algorithm_Parameters::mutation_max_step
 * The maximum amount by which an individual's value can change during a mutation.
 *
 * @var Algorithm_Parameters::early_stopping_generations
 * The number of generations without improvement after which the algorithm should stop early.
 *
 * @var Algorithm_Parameters::mutation_type_rate
 * A vector that determines the relative probabilities of different types of mutations.
 *
 * @var Algorithm_Parameters::crossover_type_rate
 * A vector that determines the relative probabilities of different types of crossovers.
 */
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
/**
 * @struct Individual
 * @brief Represents an individual in the population.
 *
 * @var Individual::vector
 * A vector representing the state of the individual.
 *
 * @var Individual::fitness_val
 * The fitness value of the individual, calculated using a provided fitness function.
 */
struct Individual {
    std::vector<int> vector;
    double fitness_val;
};
/**
 * @brief Evaluate the performance of a given circuit vector.
 * @param vector_size The size of the circuit vector.
 * @param circuit_vector A pointer to the circuit vector.
 * @return A double value representing the performance of the circuit vector.
 */
double Evaluate_circuit(int vector_size, int* circuit_vector);
/**
 * @brief Initialize a population for the genetic algorithm.
 * @param vector_size The size of the vectors representing individuals in the population.
 * @param gen The random number generator.
 * @param uniform_int A uniform integer distribution.
 * @param circuit The circuit that is being optimized.
 * @param parameters The parameters of the genetic algorithm.
 * @return A vector of individuals representing the initial population.
 */
std::vector<Individual> InitializePopulation(int vector_size, std::mt19937& gen, std::uniform_int_distribution<>& uniform_int, CCircuit& circuit, Algorithm_Parameters& parameters);
/**
 * @brief Calculate the fitness of each individual in a population.
 * @param parents A vector of individuals for which to calculate fitness.
 * @param func A function that calculates fitness.
 */
void CalculateFitness(std::vector<Individual>& parents, double(&func) (int, int*));
/**
 * @brief Sorts individuals in a population by fitness.
 * @param parents A vector of individuals to be sorted.
 */
void SortParentsByFitness(std::vector<Individual>& parents);
/**
 * @brief Normalize the fitness values in a population.
 * @param parents A vector of individuals whose fitness values will be normalized.
 * @return A vector of normalized fitness values.
 */
std::unique_ptr<std::vector<double>> NormalizeFitness(std::vector<Individual>& parents);
/**
 * @brief Create a cumulative distribution function from a normalized distribution.
 * @param normalized A pointer to the normalized distribution.
 * @return A pointer to the cumulative distribution function.
 */
std::unique_ptr<std::vector<double>> MakeCDF(std::unique_ptr<std::vector<double>>& normalized);
/**
 * @brief Select two parents for reproduction using a cumulative distribution function.
 * @param parents A vector of individuals from which to select parents.
 * @param cdf A pointer to the cumulative distribution function.
 * @param gen The random number generator.
 * @param uniform_real A uniform real distribution.
 * @return A pair of individuals that have been selected as parents.
 */
std::pair<Individual, Individual> SelectParents(std::vector<Individual>& parents, std::unique_ptr<std::vector<double>>& cdf, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real);
/**
 * @brief Perform a crossover operation on two parents to produce offspring.
 * @param selected_parentX The first parent.
 * @param selected_parentY The second parent.
 * @param gen The random number generator.
 * @param uniform_real A uniform real distribution.
 * @param parameters The parameters of the genetic algorithm.
 * @param vector_size The size of the vectors representing individuals in the population.
 * @return A pair of individuals representing the offspring.
 */
std::pair<Individual, Individual> Crossover(Individual& selected_parentX, Individual& selected_parentY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, Algorithm_Parameters& parameters, int vector_size);
/**
 * @brief Perform mutation on two individuals.
 * @param childX The first individual.
 * @param childY The second individual.
 * @param gen The random number generator.
 * @param uniform_real A uniform real distribution.
 * @param uniform_int A uniform integer distribution.
 * @param uniform_mutation_step A uniform real distribution used to generate mutation steps.
 * @param parameters The parameters of the genetic.
 */
void Mutation(Individual& childX, Individual& childY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, std::uniform_int_distribution<>& uniform_int, std::uniform_real_distribution<>& uniform_mutation_step, Algorithm_Parameters& parameters, int vector_size);
/**
 * @brief Add valid children to the population.
 * @param children A vector of individuals that will be updated with valid children.
 * @param childX The first child to be added if it's valid.
 * @param childY The second child to be added if it's valid.
 * @param circuit The circuit that is being optimized.
 */
void AddChildren(std::vector<Individual>& children, Individual& childX, Individual& childY, CCircuit& circuit);
/**
 * @brief Run the genetic algorithm to optimize the circuit.
 * @param vector_size The size of the vectors representing individuals in the population.
 * @param func A function that calculates fitness.
 * @param parameters The parameters of the genetic algorithm.
 * @return The fitness value of the best individual found.
 */
double optimize(int vector_size, int* best_vector, double(&func) (int, int*), Algorithm_Parameters parameters);


#endif 