#ifndef CUnit_H
#define CUnit_H

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

#pragma once


class CUnit {
public:
    // ID numbers
    int ID_num; // ID number of the CUnit
    int ID_num_conc; // ID number of the concentrate destination
    int ID_num_tail; // ID number of the tailing destination

    // The following varibales iwll be used for support whne chekcing the validity of the circuit
    bool mark;
    int conc_num;
    int tails_num;

    // Calculation data
    double tau; // Spatial velocity
    double Rg; // Recovery of Gerardium
    double Rw; // Recovery of waste
    // Inlet data
    double Fti; // Total mass flowrate at the inlet [kg/s]
    double Fgi; // Gerardium mass flowrate at the inlet [kg/s]
    double Fwi; // Waste mass flowrate at the inlet [kg/s]

    double Cgi; // Concentration of Gerardium at the inlet
    double Cwi; // Concentration of waste at the inlet

    // Concentrate data
    double Ftc; // Total mass flowrate at the concentrate [kg/s]
    double Fgc; // Gerardium mass flowrate at the concentrate [kg/s]
    double Fwc; // Waste mass flowrate at the concentrate [kg/s]

    double Cgc; // Concentration of Gerardium at concentrate
    double Cwc; // Concentration of waste at the concentrate

    // Tailings data
    double Ftt; // Total mass flowrate at the tailings [kg/s]
    double Fgt; // Gerardium mass flowrate at the tailings [kg/s]
    double Fwt; // Waste mass flowrate at the tailings [kg/s]

    double Cgt; // Concentration of Gerardium at the tailing
    double Cwt; // Concentration of Waste at the tailing

    void ConcentrationCalc();
    void ResidenceTime();
    void ReactionComp();
    void OutletCalc();
    void CheckMassBalance();

    void SolveCUnit();
    void PrintCUnit();
    std::vector<double> ReturnOutlets();

    // Default Constructor
    CUnit() : ID_num(0), ID_num_conc(0), ID_num_tail(0), mark(false),
              tau(0.0), Rg(0.0), Rw(0.0),
              Fti(0.0), Fgi(0.0), Fwi(0.0),
              Cgi(0.0), Cwi(0.0),
              Ftc(0.0), Fgc(0.0), Fwc(0.0),
              Cgc(0.0), Cwc(0.0),
              Ftt(0.0), Fgt(0.0), Fwt(0.0),
              Cgt(0.0), Cwt(0.0) {}

    // Constructor with initial Gerdasium and Waste flowrates
    CUnit(int ID_num_, double Fgi_, double Fwi_) : ID_num(ID_num_), ID_num_conc(0), ID_num_tail(0), mark(false),
                                                   tau(0.0), Rg(0.0), Rw(0.0),
                                                   Fti(Fgi_ + Fwi_), Fgi(Fgi_), Fwi(Fwi_),
                                                   Cgi(0.0), Cwi(0.0),
                                                   Ftc(0.0), Fgc(0.0), Fwc(0.0),
                                                   Cgc(0.0), Cwc(0.0),
                                                   Ftt(0.0), Fgt(0.0), Fwt(0.0),
                                                   Cgt(0.0), Cwt(0.0) {}
};



#endif

