#include "CUnit.h"

bool isClose(double a, double b, double tolerance) {
    return std::abs(a - b) <= tolerance;
}

void CUnit::SolveCUnit() {
    ResidenceTime();
    ReactionComp();
    OutletCalc();
    ConcentrationCalc();
}

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

void CUnit::ReactionComp() {
    double kg = 0.005; // Reaction rate constant for Gerardium [1/s]
    double kw = 0.0005; // Reaction rate constant for waste [1/s]

    Rg = kg * tau / (1 + kg * tau);
    Rw = kw * tau / (1 + kw * tau);
}

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

std::vector<double> CUnit::ReturnOutlets() {
    std::vector<double> Outlets = { Fgc, Fwc, Fgt, Fwt };
    return Outlets;
}

void CUnit::CheckMassBalance() {
    double tol = 1e-10;
    // Global Total mass balance
    if (isClose(Fti, Ftc + Ftt, tol)) {
        std::cout << "\nGlobal Total mass balance is correct" << std::endl;
    }
    else {
        std::cout << "\nGlobal Total mass balance is incorrect" << std::endl;
    }
    // Global Gerardium mass balance
    if (isClose(Fgi, Fgc + Fgt, tol)) {
        std::cout << "Global Gerardium mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global Gerardium mass balance is incorrect" << std::endl;
    }
    // Global Waste mass balance
    if (isClose(Fwi, Fwc + Fwt, tol)) {
        std::cout << "Global Waste mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global Waste mass balance is incorrect" << std::endl;
    }
    // Mass balance at the inlet
    if (isClose(Fti, Fgi + Fwi, tol)) {
        std::cout << "Mass balance at the inlet is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the inlet is incorrect" << std::endl;
    }
    // Mass balance at the concentrate
    if (isClose(Ftc, Fgc + Fwc, tol)) {
        std::cout << "Mass balance at the concentrate is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the concentrate is incorrect" << std::endl;
    }
    // Mass balance at the tailing
    if (isClose(Ftt, Fgt + Fwt, tol)) {
        std::cout << "Mass balance at the tailing is correct" << std::endl;
    }
    else {
        std::cout << "Mass balance at the tailing is incorrect" << std::endl;
    }
    // Global general mass balance
    if (isClose(Fgi + Fwi, Fgc + Fgt + Fwc + Fwt, tol)) {
        std::cout << "Global general mass balance is correct" << std::endl;
    }
    else {
        std::cout << "Global general mass balance is incorrect" << std::endl;
    }
}