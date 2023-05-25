#include <iostream>

#include "CCircuit.h"
#include "CUnit.h"
#include "CSimulator.h"

int main(int argc, char* argv[]) {
    std::cout << "Test CUnit" << std::endl;
    int ID_num = 0;
    double Fgi = 20;
    double Fwi = 80;
    CUnit TestCUnit(0, 20, 80);
    // Checking the calculated spatial velocity
    TestCUnit.ResidenceTime();
    double correct_tau = 30; // Correct spatial velocity
    double tol = 1e-2; // Tolerance
    if (abs(TestCUnit.tau - correct_tau) < tol) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    // Checking the calculated reaction rates
    TestCUnit.ReactionComp();
    double correct_Rg = 0.130; // Correct reaction rate of Gerardium
    double correct_Rw = 0.0148; // Correct reaction rate of waste
    if ((abs(TestCUnit.Rg - correct_Rg) < tol) && (abs(TestCUnit.Rw - correct_Rw) < tol)) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    // Checking the calculations and outlet values
    TestCUnit.OutletCalc();
    std::vector<double> outlets = TestCUnit.ReturnOutlets();
    double correct_Fgc = 2.61; // Correct Gerardium concentrate mass flow rate
    double correct_Fwc = 1.18; // Correct Waste concentrate mass flow rate
    double correct_Fgt = 17.39; // Correct Gerardium tailing mass flow rate
    double correct_Fwt = 78.82; // Correct Waste tailing mass flow rate
    if ((abs(outlets[0] - correct_Fgc) < tol) && (abs(outlets[1] - correct_Fwc) < tol) && (abs(outlets[2] - correct_Fgt) < tol) && (abs(outlets[3] - correct_Fwt) < tol)) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    // The CUnit has a built in mass balance check, so we will call it in order to test the calculations
    if (TestCUnit.CheckMassBalance() == 0) {
        std::cout << "pass\n";
    }
    else {
        std::cout << "fail\n";
        return 1;
    }
    return 0;
}