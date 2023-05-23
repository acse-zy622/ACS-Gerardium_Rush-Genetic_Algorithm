#include "CCircuit.h"
#include "CSimulator.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Circuit_Parameters default_circuit_parameters = {0.01, 1000};

int dummy_answer_vector[] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9,
                             10, 11, 10, 11, 10, 11, 10, 11};

double Evaluate_Circuit(int vector_size, int* circuit_vector, struct Circuit_Parameters parameters) {
//This function takes a circuit vector and returns a performance value.
//The current version of the function is a dummy function that returns
// a performance value based on how close the circuit vector is to a predetermined answer vector.
    vector<int> CCircuit_vector;
    vector<double> flowrates_c;
    vector<double> new_flowrates_c;
    vector<double> flowrates_t;
    vector<double> new_flowrates_t;
    for(int n = 0; n < vector_size; n++)
    {
        CCircuit_vector.emplace_back(circuit_vector[n]);
    }

    CCircuit MyCCircuit((vector_size-1)/2, CCircuit_vector);
    MyCCircuit.FillIDs();
    double Fgi = 10;
    double Fwi = 100;
    MyCCircuit.InitialFlowrates(Fgi, Fwi);
    MyCCircuit.ReturnFlowrates(flowrates_c, flowrates_t);

    double max_diff = 0;
    int cnt = 0;
    while(max_diff>parameters.tolerance && cnt < parameters.max_iterations)
    {
        MyCCircuit.SolveCCircuit(); // TBC
        swap(flowrates_c, new_flowrates_c);
        swap(flowrates_t, new_flowrates_t);
        MyCCircuit.ReturnFlowrates(new_flowrates_c, new_flowrates_t);
        for(int n = 0; n < new_flowrates_c.size(); n++)
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
    return MyCCircuit.ReturnProfit();
}

// overloads
double Evaluate_Circuit(int vector_size, int* circuit_vector){
    return Evaluate_Circuit(vector_size, circuit_vector, default_circuit_parameters);
};

// Other functions and variables to evaluate a real circuit.

int main()
{
    int vector[11] = {4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3};
    cout << Evaluate_Circuit(11, vector) << endl;

    return 0;
}