#ifndef CCircuit_H
#define CCircuit_H

#pragma once

/** header file for the circuit 
 * 
 * This header file defines the function that will be used to generate and validate the circuit
*/



#include "CUnit.h"
#include <vector>
#include <iostream>
#include <algorithm>

class CCircuit {
public:
    int num_units;
    double system_mineral_input;
    double system_waste_input;
    std::vector<double> recycle_mineral;
    std::vector<double> recycle_waste;
    std::vector<int> CCircuit_vector;
    std::vector<CUnit> units;

    CCircuit();
    CCircuit(int num_units);
    CCircuit(int num_units_, const std::vector<int>& CCircuit_vector_);
    void FillIDs();
    void SystemFlowrates(const double Fgi_, const double Fwi_);
    void SolveCCircuit();
    void ReturnFlowrates(std::vector<double>& Flowrates_g, std::vector<double>& Flowrates_w);
    double ReturnProfit();
    void ResetFlowrates();
    void SetInlet();

    // Check validity options
    bool Check_Validity(int* circuit_vector);
    void mark_units(int unit_num);
};
#endif
