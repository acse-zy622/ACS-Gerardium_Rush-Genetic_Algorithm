/**
*@brief header file to perform hyperparamter search
**/
#ifndef HyperPSearch_H
#define HyperPSearch_H
#include "Genetic_Algorithm.h"

Algorithm_Parameters generateRandomParameters(int vector_size);
void randomSearch(int iterCount, int* best_vector, int vector_size);


#endif
