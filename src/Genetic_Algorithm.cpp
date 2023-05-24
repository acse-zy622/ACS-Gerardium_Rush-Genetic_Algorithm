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
double Evaluate_circuit(int vector_size, int* circuit_vector)
{
    double Performance = 0.0;
    for (int i = 0; i < vector_size; i++)
    {
        std::vector<int>answer_vector = { 0,1,2,3,4,3,1,2,0,0,1};//answer_vector is a predetermined answer vector (same size as circuit_vector)
        Performance += (20 - abs(circuit_vector[i] - answer_vector[i]) * 100.0);
    }
    return Performance;
}


int optimize(int vector_size, double(&func) (int, int*), Algorithm_Parameters parameters) {

    // Set random and initial distributions
    std::vector<double>performance_time;
    CCircuit circuit((vector_size-1)/2);
    int generation = 0;
    int stagnant_generations = 0;
    double best_fitness = -1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> uniform_real(0.0, 1.0);
    std::uniform_int_distribution<> uniform_int(0, 1 + ((vector_size - 1) / 2));
    std::uniform_real_distribution<> uniform_mutation_step(-parameters.mutation_max_step, parameters.mutation_max_step);
    std::uniform_int_distribution<>uniform_int_cross(1, vector_size / 2);
    



    // Initialize random collection of circuits
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
    std::cout << "initialized within "<< count_total << " iterations" << std::endl;
    std::cout << "successful loaded 100 parents!" << std::endl;


    // Do iterations
    while (generation < parameters.generation_size) {
        std::cout << generation << std::endl;// Calculate the fitness value for each individual in parents

        ////////////////////////start_time///////////////////////
        auto func_start = std::chrono::high_resolution_clock::now();


        for (int i = 0; i < parents.size(); i++) {
            parents[i].fitness_val = func(parents[i].vector.size(), parents[i].vector.data());
        }

        ////////////////////////ending_time//////////////////////
        auto func_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> func_elapsed = func_end - func_start;
        //std::cout << "Time taken by func: " << func_elapsed.count() << " seconds" << std::endl;
        performance_time.push_back(func_elapsed.count());
        
        

        // Sort parents by fitness in decreasing order
        std::sort(parents.begin(), parents.end(), [](const Individual& x, const Individual& y) {
            return x.fitness_val > y.fitness_val; });
        
        std::cout << parents[0].fitness_val << std::endl;
        // Check if the best fitness has improved
        if (parents[0].fitness_val > best_fitness) {
            best_fitness = parents[0].fitness_val;
            stagnant_generations = 0;  // reset stagnant_generations
        }
        else {
            stagnant_generations++;  // increase stagnant_generations
        }

        if (stagnant_generations >= parameters.early_stopping_generations) {
            std::cout << "Early stopping in " << generation << " iterations" << std::endl;
            break;
        }

        // Take the best vector into the child list unchanged
        std::vector<Individual> children;
        children.push_back(parents[0]);

        // Fristly normalizing fitness values
        auto normalized = std::make_unique<std::vector<double>>(parents.size());
        double total = 0;
        for (const auto& parent : parents) {
            total += parent.fitness_val;
        }
        for (int i = 0; i < parents.size(); i++) {
            (*normalized)[i] = parents[i].fitness_val / total;
        }

        // Make cdf such that we can select pairs based on cdf table
        auto cdf = std::make_unique<std::vector<double>>(parents.size());
        (*cdf)[0] = (*normalized)[0];
        for (int i = 1; i < parents.size(); i++) {
            (*cdf)[i] = (*cdf)[i - 1] + (*normalized)[i];
        }

       
        while (children.size() < parents.size()) {
            
            // Select parents 
            // Random numbers for parent selection
            double r_num1 = uniform_real(gen);
            double r_num2 = uniform_real(gen);

            std::sort(parents.begin(), parents.end(), [](const Individual& x, const Individual& y) {
                return x.fitness_val < y.fitness_val; });

            // Get the lower bound of cdf such that we are accessing the index plus 1
            auto it_1 = std::lower_bound(cdf->begin(), cdf->end(), r_num1);
            auto it_2 = std::lower_bound(cdf->begin(), cdf->end(), r_num2);

            // get the exact index by minus 1
            Individual& selected_parentX = parents[it_1 - cdf->begin()];
            Individual& selected_parentY = parents[it_2 - cdf->begin()];

            // Create children as copies of parents
            Individual childX = selected_parentX;
            Individual childY = selected_parentY;


            // Adaptive crossover for further considerations?
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

            // Mutation
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

            // Check the validity of the circuits as well as comparsion for adaptive crossover
            if (circuit.Check_Validity(childX.vector.data())) {
                children.push_back(childX);
            }
            if (circuit.Check_Validity(childY.vector.data())) {
                children.push_back(childY);
            }
        }

        normalized.release();
        cdf.release();
        parents = children;
        generation++;

    }

 
    double total_time = std::accumulate(performance_time.begin(), performance_time.end(), 0.0);
    std::cout << "Overall performance time: " << total_time << " seconds" <<std::endl;


    // Output the input vector with the best solution found
    std::cout << "Best solution found: "<<std::endl;
    for (int i = 0; i < parents[0].vector.size(); ++i) {
        std::cout << parents[0].vector[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Best fitness value is: " << std::endl;
    return parents[0].fitness_val;
}
