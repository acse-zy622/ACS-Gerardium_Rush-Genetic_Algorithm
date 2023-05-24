/******************************************
 * CSimulator Class Source File
 * By Jiachen Dong and Jiajia Zheng
 ******************************************/

#include "CCircuit.h"
#include "CSimulator.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Define default circuit parameters: tolerance and maximum iterations
struct Circuit_Parameters default_circuit_parameters = {0.01, 1000};

/**
 *
 * @param vector_size the size of the circuit vector
 * @param circuit_vector the pointer of the circuit vector array
 * @param parameters circuit parameters: tolerance and maximum iterations
 * @return The monetary value of the final concentrate.
 */
double Evaluate_Circuit(int vector_size, int* circuit_vector, struct Circuit_Parameters parameters) {
    vector<int> CCircuit_vector;  // Define the circuit vector in "vector" form
    for(int n = 0; n < vector_size; n++)  // Assign values to circuit vector
    {
        CCircuit_vector.emplace_back(circuit_vector[n]);
    }

    vector<double> flowrates_g;  // Define flow rate of gerardium of each unit
    vector<double> new_flowrates_g;  // Define new flow rate of gerardium of each unit
    vector<double> flowrates_w;  // Define flow rate of waste of each unit
    vector<double> new_flowrates_w;  // Define new flow rate of waste of each unit

    CCircuit MyCCircuit((vector_size-1)/2, CCircuit_vector);  // Define the circuit
    MyCCircuit.FillIDs();  // Set IDs of each unit in this circuit

    double Sg = 10;  // Feed mineral
    double Sw = 100;  // Feed waste
    MyCCircuit.SystemFlowrates(Sg, Sw);  // Initialize the flow rate of the circuit
    MyCCircuit.ReturnFlowrates(flowrates_g, flowrates_w);  // Get initial flow rate of concentrates and tailings


    double max_diff = parameters.tolerance;  // Record maximum change of flow rate
    int cnt = 0;  // Count while loop
    while(max_diff>=parameters.tolerance && cnt < parameters.max_iterations)
    {
        MyCCircuit.SolveCCircuit(); // Solve mass balance of the circuit
        MyCCircuit.ReturnFlowrates(new_flowrates_g, new_flowrates_w);  // Get updated flow rate of concentrates and tailings
        
        for(int n = 0; n < new_flowrates_g.size(); n++)  // Capture maximum change of flow rate
        {
            if(abs(new_flowrates_g[n] - flowrates_g[n]) > max_diff)
            {
                max_diff = abs(new_flowrates_g[n] - flowrates_g[n]);
            }
            if(abs(new_flowrates_w[n] - flowrates_w[n]) > max_diff)
            {
                max_diff = abs(new_flowrates_w[n] - flowrates_w[n]);
            }
        }
        swap(flowrates_g, new_flowrates_g);  // Quick assign values from new_flowrates_c to flowrates_c
        swap(flowrates_w, new_flowrates_w);  // Quick assign values from new_flowrates_t to flowrates_t
        cnt++;
    }
    return MyCCircuit.ReturnProfit();  // Calculate and return monetary value of the final concentrate
}

// Overload
double Evaluate_Circuit(int vector_size, int* circuit_vector){
    return Evaluate_Circuit(vector_size, circuit_vector, default_circuit_parameters);
};


// Uncomment the following code to run a simple example
// int main()
// {
//     int vector[11] = {4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3};
//     cout << Evaluate_Circuit(11, vector) << endl;
//     return 0;
// }