#include <iostream>

#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"

int main(int argc, char* argv[]) {
    std::cout << "Test CCircuit" << std::endl;
    // Defining the circuit
    int num_units = 5;
    std::vector<int> vector = { 4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3 };
    CCircuit TestCCircuit(num_units, vector);
    int SystemFeedID = vector[0];
    // Testing the IDs are correctly assigned
    TestCCircuit.FillIDs();
    std::vector<int> CorrectIDs = { 0, 1, 2, 3, 4 };
    std::vector<int> Correct_Conc_IDs = { 5, 2, 0, 1, 1 };
    std::vector<int> Correct_Tail_IDs = { 1, 4, 1, 6, 3 };
    for (int i = 0; i < num_units; i++) {
        if ((TestCCircuit.units[i].ID_num == CorrectIDs[i]) || (TestCCircuit.units[i].ID_num_conc == Correct_Conc_IDs[i]) || (TestCCircuit.units[i].ID_num_tail == Correct_Tail_IDs[i])) {
            std::cout << "pass\n";
        }
        else {
            std::cout << "fail\n";
            return 1;
        }
    }
    // Checking the system inlet flowrates are correctly assigned
    double Sg = 10;
    double Sw = 100;
    TestCCircuit.SystemFlowrates(Sg, Sw);
    if ((TestCCircuit.system_mineral_input == Sg) && (TestCCircuit.system_waste_input == Sw)) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    // Checkign the inlets for the iteration are properly set
    TestCCircuit.SetInlet();
    std::vector<double> Fgi_Flowrates = { 0, 0, 0, 0, 10, 0, 0 };
    std::vector<double> Fwi_Flowrates = { 0, 0, 0, 0, 100, 0, 0 };
    std::vector<double> Fti_Flowrates = { 0, 0, 0, 0, 110, 0, 0 };
    for (int i = 0; i < num_units; i++) {
        if ((TestCCircuit.units[i].Fgi != Fgi_Flowrates[i]) || (TestCCircuit.units[i].Fwi != Fwi_Flowrates[i]) || (TestCCircuit.units[i].Fti != Fti_Flowrates[i])) {
            std::cout << "fail\n";
            return 1;
        }
        else {
            std::cout << "pass\n";
        }
    }
    // We run the whole circuit for one iteration and check the proper recycles are obtained
    TestCCircuit.SolveCCircuit();
    double tol = 1e-1;
    std::vector<double> Correct_Recycle_g = { 0, 1.2, 0, 8.8, 0, 0, 0 };
    std::vector<double> Correct_Recycle_w = { 0, 1.35, 0, 98.65, 0, 0, 0 };
    for (int i = 0; i < num_units + 2; i++) {
        if ((abs(Correct_Recycle_g[i] - TestCCircuit.recycle_mineral[i]) < tol) || (abs(Correct_Recycle_w[i] - TestCCircuit.recycle_waste[i]) < tol)) {
            std::cout << "pass\n";
        }
        else {
            std::cout << "fail\n";
            return 1;
        }
    }
    // Checking the profit for one iteration
    double Profit;
    Profit = TestCCircuit.ReturnProfit();
    double Real_Profit = 0.0;
    if (Profit == Real_Profit) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    // Checking the proper flowrates are correctly returned
    std::vector<double> Store_Flowrate_g;
    std::vector<double> Store_Flowrate_w;
    std::vector<double> Correct_Flowrate_g = { 0, 0, 0, 0, 10, 0, 0 };
    std::vector<double> Correct_Flowrate_w = { 0, 0, 0, 0, 100, 0, 0 };
    TestCCircuit.ReturnFlowrates(Store_Flowrate_g, Store_Flowrate_w);
    for (int i = 0; i < num_units + 2; i++) {
        if ((Store_Flowrate_g[i] != Correct_Flowrate_g[i]) || (Store_Flowrate_w[i] != Correct_Flowrate_w[i])) {
            return 1;
        }
        else {
            std::cout << "pass\n";
        }
    }
    // Checking the flowrates are properly reset
    TestCCircuit.ResetFlowrates();
    for (int i = 0; i < num_units + 2; i++) {
        if ((TestCCircuit.units[i].Fgi != 0) || (TestCCircuit.units[i].Fwi != 0) || (TestCCircuit.units[i].Fgc != 0) || (TestCCircuit.units[i].Fwc != 0) || (TestCCircuit.units[i].Fgt != 0) || (TestCCircuit.units[i].Fwt != 0)) {
            std::cout << "fail\n";
            return 1;
        }
        else {
            std::cout << "pass\n";
        }
    }
}

