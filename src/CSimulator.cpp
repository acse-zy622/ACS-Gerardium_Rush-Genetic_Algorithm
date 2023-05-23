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

    vector<double> flowrates_c;  // Define flow rate of concentrates of each unit
    vector<double> new_flowrates_c;  // Define new flow rate of concentrates of each unit
    vector<double> flowrates_t;  // Define flow rate of tailings of each unit
    vector<double> new_flowrates_t;  // Define new flow rate of tailings of each unit

    CCircuit MyCCircuit((vector_size-1)/2, CCircuit_vector);  // Define the circuit
    MyCCircuit.FillIDs();  // Set IDs of each unit in this circuit

    double Fgi = 10;  // Feed mineral
    double Fwi = 100;  // Feed waste
    MyCCircuit.InitialFlowrates(Fgi, Fwi);  // Initialize the flow rate of the circuit
    MyCCircuit.ReturnFlowrates(flowrates_c, flowrates_t);  // Get initial flow rate of concentrates and tailings

    double max_diff = 0;  // Record maximum change of flow rate
    int cnt = 0;  // Count while loop
    while(max_diff>parameters.tolerance && cnt < parameters.max_iterations)
    {
        MyCCircuit.SolveCCircuit(); // Solve mass balance of the circuit
        swap(flowrates_c, new_flowrates_c);  // Quick assign values from new_flowrates_c to flowrates_c
        swap(flowrates_t, new_flowrates_t);  // Quick assign values from new_flowrates_t to flowrates_t
        MyCCircuit.ReturnFlowrates(new_flowrates_c, new_flowrates_t);  // Get updated flow rate of concentrates and tailings

        for(int n = 0; n < new_flowrates_c.size(); n++)  // Capture maximum change of flow rate
        {
            if(abs(new_flowrates_c[n] - flowrates_c[n]) > max_diff)
            {
                max_diff = abs(new_flowrates_c[n] - flowrates_c[n]);
            }
            if(abs(new_flowrates_t[n] - flowrates_t[n]) > max_diff)
            {
                max_diff = abs(new_flowrates_t[n] - flowrates_t[n]);
            }
        }
    }
    return MyCCircuit.ReturnProfit();  // Calculate and return monetary value of the final concentrate
}

// Overload
double Evaluate_Circuit(int vector_size, int* circuit_vector){
    return Evaluate_Circuit(vector_size, circuit_vector, default_circuit_parameters);
};


// Uncomment the following code to run a simple example
//int main()
//{
//    int vector[11] = {4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3};
//    cout << Evaluate_Circuit(11, vector) << endl;
//    return 0;
//}