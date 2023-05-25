#include <iostream>

#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"

int main(int argc, char* argv[]) {
    std::cout << "Test CSimulator" << std::endl;
    // Defining the circuit
    int vector[11] = { 4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3 };
    // Solving the circuit and getting the profit
    double CircuitProfit = Evaluate_Circuit(sizeof(vector) / sizeof(vector[0]), vector);
    // Checking the profit
    double tol = 1e-3;
        if (abs(CircuitProfit - 107.204) < tol) {
            std::cout << "pass\n";
        }
        else {
            std::cout << "fail\n";
            return 1;
        }
}
