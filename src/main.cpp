#include <iostream>

#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"
#include "Genetic_Algorithm.h"


int main(int argc, char * argv[])
{

    // set things up
    int vector[11] = {0,1,2,3,4,5,6,7,8,9,10};  

    // run your code
    optimize(11, vector, Evaluate_Circuit);

    // generate final output
    std::cout << Evaluate_Circuit(11, vector);

    return 0;
}
