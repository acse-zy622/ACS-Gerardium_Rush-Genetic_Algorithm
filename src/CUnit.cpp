#include "CUnit.h"

/**
*@brief Checks if two double values are close within a specified tolerance.
*This function compares two double values and determines if they are close
*to each other within the specified tolerance. The comparison is performed
*using the absolute difference between the two values.
*@param a The first double value to compare.
*@param b The second double value to compare.
*@param tolerance The tolerance value within which the two values are considered close.
*@return true if the absolute difference between the two values is less than or equal to the tolerance, false otherwise.
*/
bool isClose(double a, double b, double tolerance) {
    return std::abs(a - b) <= tolerance;
}


/**

*@brief Solves the CUnit by performing necessary calculations.
*This function solves the CUnit by performing the following steps:
*Calculates the residence time.
*Performs reaction composition calculations.
*Calculates the outlet values.
*Calculates the concentration.
*@note This function assumes that the CUnit and its related data have been properly initialized.
*@see CUnit, ResidenceTime(), ReactionComp(), OutletCalc(), ConcentrationCalc()
*/
void CUnit::SolveCUnit() {
    ResidenceTime();
    ReactionComp();
    OutletCalc();
    ConcentrationCalc();
}

/**
*@brief Prints the CUnit information and performs a mass balance check.
*This function prints various components and flow rates at the inlet, concentrate, and tailing of the CUnit. It also displays the spatial time and reaction rates. 
*Finally, it calls the CheckMassBalance() function to perform a mass balance check.
*@note This function assumes that the CUnit and its related data have been properly initialized.
*@see CUnit, CheckMassBalance()

*/
void CUnit::PrintCUnit() {
    // Spatial time and reaction rates
    std::cout << "\nSpatial time: " << tau << std::endl;
    std::cout << "Rg: " << Rg << " and " << "Rw: " << Rw << std::endl;
    // Components at inlet
    std::cout << "\nThe Total flowrate at the inlet is: " << Fti << std::endl;
    std::cout << "The Gerardium flowrate at the inlet is: " << Fgi << std::endl;
    std::cout << "The Waste flowrate at the inlet is: " << Fwi << std::endl;
    std::cout << "The Gerardium concentration at the inlet is: " << Cgi << std::endl;
    std::cout << "The Waste concentration at the inlet is: " << Cwi << std::endl;
    // Components ar concentrate
    std::cout << "\nThe Total flowrate at the concentrate is: " << Ftc << std::endl;
    std::cout << "The Gerardium flowrate at the concentrate is: " << Fgc << std::endl;
    std::cout << "The Waste flowrate at the concentrate is: " << Fwc << std::endl;
    std::cout << "The Gerardium concentration at the inlet is: " << Cgc << std::endl;
    std::cout << "The Waste concentration at the inlet is: " << Cwc << std::endl;
    // Components at tailing
    std::cout << "\nThe Total flowrate at the tailing is: " << Ftt << std::endl;
    std::cout << "The Gerardium flowrate at the tailing is: " << Fgt << std::endl;
    std::cout << "The Waste flowrate at the tailing is: " << Fwt << std::endl;
    std::cout << "The Gerardium concentration at the inlet is: " << Cgt << std::endl;
    std::cout << "The Waste concentration at the inlet is: " << Cwt << std::endl;
    // We call the CheckMassBalance
    CheckMassBalance();
}

/**
*@brief Calculates the concentrations of components in the CUnit.
*This function calculates the concentrations of components in the CUnit by dividing the corresponding flow rates by the total flow rate.

*@note This function assumes that the CUnit and its related data have been properly initialized.

*@see CUnit
*/
void CUnit::ConcentrationCalc() {
    // Concentrations at the inlet
    Cgi = Fgi / Fti;
    Cwi = Fwi / Fti;
    // Concentrations at the concentrate
    Cgc = Fgc / Ftc;
    Cwc = Fwc / Ftc;
    // Concentrations at the tailing
    Cgt = Fgt / Ftt;
    Cwt = Fwt / Ftt;
}

/**
*@brief Calculates the residence time of the CUnit.

*This function calculates the residence time of the CUnit based on the given parameters: gamma, V, pg, pw, Fgi, and Fwi.

*@note This function assumes that the CUnit and its related data have been properly initialized.

*@see CUnit
*/
void CUnit::ResidenceTime() {
    double gamma = 0.1; // Ratio of solid content to feed content
    double V = 10; // Volume of the tank [m^3]
    double pg = 3000; // Density of Gerardium [kg/m^3]
    double pw = 3000; // Density of waste [kg/m^3]

    if (Fgi == 0 && Fwi == 0) {
        tau = 0;
    }
    else {
        tau = gamma * V * 1 / (Fgi / pg + Fwi / pw);
    }

}

/**
 *@brief Calculates the reaction components.
 *
 * This method calculates the reaction components based on the reaction rate constants
 * for Gerardium and waste.
 */
void CUnit::ReactionComp() {
    double kg = 0.005; // Reaction rate constant for Gerardium [1/s]
    double kw = 0.0005; // Reaction rate constant for waste [1/s]

    Rg = kg * tau / (1 + kg * tau);
    Rw = kw * tau / (1 + kw * tau);
}

/**
 *@brief Calculates the outlet components.
 *
 * This method calculates the outlet components based on the reaction rates
 * and the input components.
 */
void CUnit::OutletCalc() {
    if (Rg == 0 && Rw == 0) {
        Fgc = 1.e-10;
        Fwc = 1.e-10;
        Fgt = 1.e-10;
        Fwt = 1.e-10;

        Ftc = 1.e-10;
        Ftt = 1.e-10;

    }
    Fgc = Fgi * Rg;
    Fwc = Fwi * Rw;
    Fgt = Fgi - Fgc;
    Fwt = Fwi - Fwc;

    Ftc = Fgc + Fwc;
    Ftt = Fgt + Fwt;
}

/**
 * @brief Returns the outlet components.
 *
 * This method returns a vector containing the outlet components: Gerardium component (Fgc),
 * waste component (Fwc), total Gerardium component (Fgt), and total waste component (Fwt).
 *
 * @return Vector containing the outlet components.
 */
std::vector<double> CUnit::ReturnOutlets() {
    std::vector<double> Outlets = { Fgc, Fwc, Fgt, Fwt };
    return Outlets;
}

/**
 *@brief Checks the mass balance of the unit.
 *
 * This method checks the mass balance of the unit by comparing the inlet and outlet
 * components and printing the results. It performs the following checks:
 * - Global Total mass balance
 * - Global Gerardium mass balance
 * - Global Waste mass balance
 * - Mass balance at the inlet
 * - Mass balance at the concentrate
 * - Mass balance at the tailing
 * - Global general mass balance
 */
int CUnit::CheckMassBalance() {
    double tol = 1e-10;
    // Global Total mass balance
    if (isClose(Fti, Ftc + Ftt, tol)) {
        std::cout << "\nGlobal Total mass balance is correct" << std::endl;
    }
    else {
        std::cout << "\nGlobal Total mass balance is incorrect" << std::endl;
        return 1;
    }
    // Global Gerardium mass balance
    if (isClose(Fgi, Fgc + Fgt, tol)) {
        std::cout << "Global Gerardium mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global Gerardium mass balance is incorrect" << std::endl;
        return 1;
    }
    // Global Waste mass balance
    if (isClose(Fwi, Fwc + Fwt, tol)) {
        std::cout << "Global Waste mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global Waste mass balance is incorrect" << std::endl;
        return 1;
    }
    // Mass balance at the inlet
    if (isClose(Fti, Fgi + Fwi, tol)) {
        std::cout << "Mass balance at the inlet is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the inlet is incorrect" << std::endl;
        return 1;
    }
    // Mass balance at the concentrate
    if (isClose(Ftc, Fgc + Fwc, tol)) {
        std::cout << "Mass balance at the concentrate is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the concentrate is incorrect" << std::endl;
        return 1;
    }
    // Mass balance at the tailing
    if (isClose(Ftt, Fgt + Fwt, tol)) {
        std::cout << "Mass balance at the tailing is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the tailing is incorrect" << std::endl;
        return 1;
    }
    // Global general mass balance
    if (isClose(Fgi + Fwi, Fgc + Fgt + Fwc + Fwt, tol)) {
        std::cout << "Global general mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global general mass balance is incorrect" << std::endl;
        return 1;
    }
    return 0;
}
