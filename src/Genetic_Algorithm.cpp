#include "Genetic_Algorithm.h"
#include "CCircuit.h"
#include <stdio.h>
#include <cmath>
#include <array>
#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <numeric>
#include "omp.h"

double Evaluate_circuit(int vector_size, int* circuit_vector)
{
    double Performance = 0.0;
    for (int i = 0; i < vector_size; i++)
    {
        std::vector<int>answer_vector = { 2,3,2,5,0,0,4,1,0,0,6 };//answer_vector is a predetermined answer vector (same size as circuit_vector)
        Performance += ((20 - abs(circuit_vector[i] - answer_vector[i])) * 100.0);
    }
    return Performance;
}

std::vector<Individual> InitializePopulation(int vector_size, std::mt19937& gen, std::uniform_int_distribution<>& uniform_int, CCircuit& circuit, Algorithm_Parameters& parameters) {
    std::vector<Individual> parents;
    int count_total = 0;
    while (parents.size() < parameters.population_size) {
        Individual parent;
        parent.vector.resize(vector_size);
        for (int j = 0; j < vector_size; j++) {
            parent.vector[j] = uniform_int(gen);
        }
        if (circuit.Check_Validity(parent.vector.data())) {
            parents.push_back(parent);
        }
        count_total++;
    }
    std::cout << "initialized within " << count_total << " iterations" << std::endl;
    std::cout << "successful loaded "<< parents.size() << " parents!" << std::endl;
    return parents;
}

void CalculateFitness(std::vector<Individual>& parents, double(&func) (int, int*)) {
    omp_set_num_threads(6);
    #pragma omp parallel for
    for (int i = 0; i < parents.size(); i++) {
        parents[i].fitness_val = func(parents[i].vector.size(), parents[i].vector.data());
    }
}

void SortParentsByFitness(std::vector<Individual>& parents) {
    std::sort(parents.begin(), parents.end(), [](const Individual& x, const Individual& y) {
        return x.fitness_val > y.fitness_val; });
}

std::unique_ptr<std::vector<double>> NormalizeFitness(std::vector<Individual>& parents) {
    auto normalized = std::make_unique<std::vector<double>>(parents.size());
    double total = 0;
    for (const auto& parent : parents) {
        total += parent.fitness_val;
    }
    for (int i = 0; i < parents.size(); i++) {
        (*normalized)[i] = parents[i].fitness_val / total;
    }
    return normalized;
}

std::unique_ptr<std::vector<double>> MakeCDF(std::unique_ptr<std::vector<double>>& normalized) {
    auto cdf = std::make_unique<std::vector<double>>(normalized->size());
    (*cdf)[0] = (*normalized)[0];
    for (int i = 1; i < normalized->size(); i++) {
        (*cdf)[i] = (*cdf)[i - 1] + (*normalized)[i];
    }
    return cdf;
}

std::pair<Individual, Individual> SelectParents(std::vector<Individual>& parents, std::unique_ptr<std::vector<double>>& cdf, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real) {
    double r_num1 = uniform_real(gen);
    double r_num2 = uniform_real(gen);
    std::sort(parents.begin(), parents.end(), [](const Individual& x, const Individual& y) {
        return x.fitness_val < y.fitness_val; });
    auto it_1 = std::lower_bound(cdf->begin(), cdf->end(), r_num1);
    auto it_2 = std::lower_bound(cdf->begin(), cdf->end(), r_num2);
    Individual& selected_parentX = parents[it_1 - cdf->begin()];
    Individual& selected_parentY = parents[it_2 - cdf->begin()];
    return std::make_pair(selected_parentX, selected_parentY);
}

std::pair<Individual, Individual> Crossover(Individual& selected_parentX, Individual& selected_parentY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, Algorithm_Parameters& parameters, int vector_size) {
    Individual childX = selected_parentX;
    Individual childY = selected_parentY;
    if (uniform_real(gen) < parameters.crossover_rate) {

        std::uniform_int_distribution<> uniform_int_size(0, vector_size - 1);
        std::uniform_int_distribution<> uniform_int_sizeM(0, (vector_size - 1) / 2);
        double crossover_type = uniform_real(gen);

        // one-point
        if (crossover_type < parameters.crossover_type_rate[0]) {
            int point = uniform_int_size(gen);

            for (int i = point; i < vector_size; i++) {
                std::swap(childX.vector[i], childY.vector[i]);
            }
        }

        // multi-point
        else if (crossover_type < parameters.crossover_type_rate[0] + parameters.crossover_type_rate[1]) {

            int num_cross_points = uniform_int_sizeM(gen);
            std::vector<int> crossing_points(num_cross_points);
            for (int i = 0; i < num_cross_points; i++) {
                crossing_points[i] = uniform_int_size(gen);
            }
            std::sort(crossing_points.begin(), crossing_points.end());

            int starting_point = 0;
            for (int i = 0; i < num_cross_points; i++) {
                if (i % 2 == 0) {
                    for (int j = starting_point; j < crossing_points[i]; j++) {
                        std::swap(childX.vector[j], childY.vector[j]);
                    }
                }
                starting_point = crossing_points[i];
            }

            //    // handle the last one.
            if (num_cross_points % 2 == 0) {
                for (int j = starting_point; j < vector_size; j++) {
                    std::swap(childX.vector[j], childY.vector[j]);
                }
            }
        }

        // uniform crossover
        else {
            for (int i = 0; i < vector_size; i++) {
                if (uniform_real(gen) <= 0.5) {
                    std::swap(childX.vector[i], childY.vector[i]);
                }
            }
        }
    }
    return std::make_pair(childX, childY);
}

void Mutation(Individual& childX, Individual& childY, std::mt19937& gen, std::uniform_real_distribution<>& uniform_real, std::uniform_int_distribution<>& uniform_int, std::uniform_real_distribution<>& uniform_mutation_step, Algorithm_Parameters& parameters, int vector_size) {
    for (int i = 0; i < std::min(childX.vector.size(), childY.vector.size()); i++) {
        if (uniform_real(gen) < parameters.mutation_rate) {
            double mutation_type = uniform_real(gen);

            // Point
            if (mutation_type < parameters.mutation_type_rate[0]) {
                // Set random mutation step from -step to step
                // Apply mutation step and ensure the result stays within range
                int mutation_step = uniform_mutation_step(gen);
                childX.vector[i] = (childX.vector[i] + mutation_step + 2 + ((vector_size - 1) / 2)) % (2 + ((vector_size - 1) / 2));
                childY.vector[i] = (childY.vector[i] + mutation_step + 2 + ((vector_size - 1) / 2)) % (2 + ((vector_size - 1) / 2));
            }
            // Rearrangement
            else {
                // Select a random interval in the vector and rearrange it
                int start = uniform_int(gen) % childX.vector.size();
                int end = uniform_int(gen) % childX.vector.size();
                if (start > end) std::swap(start, end);

                std::shuffle(childX.vector.begin() + start, childX.vector.begin() + end, gen);
                std::shuffle(childY.vector.begin() + start, childY.vector.begin() + end, gen);
            }
        }
    }
}

void AddChildren(std::vector<Individual>& children, Individual& childX, Individual& childY, CCircuit& circuit) {
    if (circuit.Check_Validity(childX.vector.data())) {
        children.push_back(childX);
    }
    if (circuit.Check_Validity(childY.vector.data())) {
        children.push_back(childY);
    }
}

double optimize(int vector_size, double(&func) (int, int*), Algorithm_Parameters parameters) {
    if (vector_size <= 0 || vector_size % 2 == 0 ) {
        std::cout << "Invalid vector size!! " << std::endl;
        return -1;
    }
    // Set random and initial distributions
    std::vector<double>performance_time;
    CCircuit circuit((vector_size - 1) / 2);
    int generation = 0;
    int stagnant_generations = 0;
    double best_fitness = -1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uniform_real(0.0, 1.0);
    std::uniform_int_distribution<> uniform_int(0, 1 + ((vector_size - 1) / 2));
    std::uniform_real_distribution<> uniform_mutation_step(-parameters.mutation_max_step, parameters.mutation_max_step-1);
    std::uniform_int_distribution<>uniform_int_cross(1, vector_size / 2);

    // Initialize random collection of circuits
    std::vector<Individual> parents = InitializePopulation(vector_size, gen, uniform_int, circuit, parameters);

    while (generation < parameters.generation_step) {
        //std::cout << generation << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        CalculateFitness(parents, func);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        performance_time.push_back(elapsed.count());

        SortParentsByFitness(parents);

        if (parents[0].fitness_val > best_fitness) {
            best_fitness = parents[0].fitness_val;
            stagnant_generations = 0;
        }
        else {
            stagnant_generations++;
        }

        if (stagnant_generations >= parameters.early_stopping_generations) {
            std::cout << "Early stopping in " << generation << " iterations" << std::endl;
            break;
        }

        std::vector<Individual> children;
        children.push_back(parents[0]);

        auto normalized = NormalizeFitness(parents);
        auto cdf = MakeCDF(normalized);

        while (children.size() < parents.size()) {
            auto [selected_parentX, selected_parentY] = SelectParents(parents, cdf, gen, uniform_real);
            auto [childX, childY] = Crossover(selected_parentX, selected_parentY, gen, uniform_real, parameters, vector_size);
            Mutation(childX, childY, gen, uniform_real, uniform_int, uniform_mutation_step, parameters, vector_size);
            AddChildren(children, childX, childY, circuit);
        }

        parents = children;
        generation++;
    }

    double total_time = std::accumulate(performance_time.begin(), performance_time.end(), 0.0);
    std::cout << "Overall performance time: " << total_time << " seconds" << std::endl;

    // Output the input vector with the best solution found
    std::cout << "Best solution found: " << std::endl;
    for (int i = 0; i < parents[0].vector.size(); ++i) {
        std::cout << parents[0].vector[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Best fitness value is: " << std::endl;
    return parents[0].fitness_val;
}