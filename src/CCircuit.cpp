#include "CCircuit.h"
#include <iostream>


// Default Constructor
CCircuit::CCircuit() : num_units(0), system_mineral_input(0.0), system_waste_input(0.0) {}

// Constructor with number of units
CCircuit::CCircuit(int num_units) {
    this->units.resize(num_units);
    for (int i = 0; i < num_units; i++) {
        this->units[i].conc_num = -1;
        this->units[i].tails_num = -1;
        this->units[i].mark = false;
    }
}

// Constructor with number of units and vector
CCircuit::CCircuit(int num_units_, const std::vector<int>& CCircuit_vector_) : num_units(num_units_), system_mineral_input(0.0), system_waste_input(0.0),
                                                                               CCircuit_vector(CCircuit_vector_) {}

double CCircuit::ReturnProfit() {
    double ProfitGerardium = 100;
    double PenalizationWaste = 500;
    return ProfitGerardium * units[num_units].Fgi - PenalizationWaste * units[num_units].Fwi;
}

void CCircuit::SystemFlowrates(double Sg_, double Sw_) {
    system_mineral_input = Sg_;
    system_waste_input = Sw_;
}

void CCircuit::ResetFlowrates() {
    for (int i = 0; i < num_units + 2; i++) {
        units[i].Fti = 0.0;
        units[i].Fgi = 0.0;
        units[i].Fwi = 0.0;
        units[i].Ftc = 0.0;
        units[i].Fgc = 0.0;
        units[i].Fwc = 0.0;
        units[i].Ftt = 0.0;
        units[i].Fgt = 0.0;
        units[i].Fwt = 0.0;
        units[i].Rg = 0.0;
        units[i].Rw = 0.0;
        units[i].tau = 0.0;
    }
}

void CCircuit::SetInlet() {
    int SystemFeedID = CCircuit_vector[0];
    for (int i = 0; i < num_units + 2; i++) {
        if (i == SystemFeedID) {
            units[SystemFeedID].Fgi = recycle_mineral[i] + system_mineral_input;
            units[SystemFeedID].Fwi = recycle_waste[i] + system_waste_input;
            units[SystemFeedID].Fti = recycle_waste[i] + recycle_mineral[i] + system_mineral_input + system_waste_input;
        }
        else {
            units[i].Fgi = recycle_mineral[i];
            units[i].Fwi = recycle_waste[i];
            units[i].Fti = recycle_waste[i] + recycle_mineral[i];
        }
    }
}


void CCircuit::ReturnFlowrates(std::vector<double>& Flowrates_g, std::vector<double>& Flowrates_w) {
    Flowrates_g.resize(num_units + 2);
    Flowrates_w.resize(num_units + 2);
    for (int i = 0; i < num_units + 2; i++) {
        Flowrates_g[i] = units[i].Fgi;
        Flowrates_w[i] = units[i].Fwi;
    }
}

void CCircuit::SolveCCircuit() {
    ResetFlowrates();
    SetInlet();
    std::fill(recycle_mineral.begin(), recycle_mineral.end(), 0.0);
    std::fill(recycle_waste.begin(), recycle_waste.end(), 0.0);
    for (int i = 0; i < num_units; i++) {
        units[i].SolveCUnit();
        int ID_Dest_Conc = units[i].ID_num_conc;
        int ID_Dest_Tail = units[i].ID_num_tail;
        std::vector<double> outlets = units[i].ReturnOutlets();
        // Assigning results fot the second and third units
        recycle_mineral[ID_Dest_Conc] += outlets[0];
        recycle_waste[ID_Dest_Conc] += outlets[1];
        recycle_mineral[ID_Dest_Tail] += outlets[2];
        recycle_waste[ID_Dest_Tail] += outlets[3];
    }
}

void CCircuit::FillIDs() {
    units.resize(num_units + 2);
    recycle_mineral.resize(num_units + 2);
    recycle_waste.resize(num_units + 2);
    std::fill(recycle_mineral.begin(), recycle_mineral.end(), 0.0);
    std::fill(recycle_waste.begin(), recycle_waste.end(), 0.0);
    int SystemFeedID = CCircuit_vector[0];
    int SystemConcentrateID = num_units;
    int SystemTailingID = num_units + 1;
    for (int i = 0; i < num_units; i++) {
        units[i].ID_num = i;
        units[i].ID_num_conc = CCircuit_vector[2 * i + 1];
        units[i].ID_num_tail = CCircuit_vector[2 * i + 2];
    }
    // Concentrate outlet of the system
    units[num_units].ID_num = SystemConcentrateID;
    units[num_units].ID_num_conc = -1;
    units[num_units].ID_num_tail = -1;
    // Tailing outlet of the system
    units[num_units + 1].ID_num = SystemTailingID;
    units[num_units + 1].ID_num_conc = -1;
    units[num_units + 1].ID_num_tail = -1;
}

// New functions from Rubab

bool validation[2] = { false, false };

bool CCircuit::Check_Validity(int* circuit_vector)
{
    validation[0] = false;
    validation[1] = false;
    int feed_num = circuit_vector[0];

    if (feed_num < 0 || feed_num >= this->units.size()) {
        return false;
    }

    for (int i = 0; i < this->units.size(); i++) {
        this->units[i].conc_num = circuit_vector[2 * i + 1];
        this->units[i].tails_num = circuit_vector[2 * i + 2];
        this->units[i].mark = false;
    }

    mark_units(feed_num);

    for (int i = 0; i < this->units.size(); i++) {
        // Every unit must be accessible from the feed
        if (!this->units[i].mark) {
            return false;
        }
        // no self-recycle
        if (this->units[i].conc_num == i || this->units[i].tails_num == i) {
            return false;
        }
        // The destination for both products from a unit should not be the same unit.
        if (this->units[i].conc_num == this->units[i].tails_num) {
            return false;
        }

        // cannot mix the two streams destination
        if (this->units[i].conc_num == this->units.size() + 1 || this->units[i].tails_num == this->units.size()) {
            return false;
        }

        // the destination cannot beyond the bound
        if (this->units[i].conc_num > (this->units.size() + 1) || this->units[i].tails_num > (this->units.size() + 1)) {
            return false;
        }
        if (this->units[i].conc_num < 0 || this->units[i].tails_num < 0) {
            return false;
        }
        // must find the two stream destionation
        if (validation[0] == false || validation[1] == false) {
            return false;
        }
    }

    return true;
}

void CCircuit::mark_units(int unit_num) {

    if (this->units[unit_num].mark) return;

    this->units[unit_num].mark = true;

    //If we have seen this unit already exit
    //Mark that we have now seen the unit
    //If conc_num does not point at a circuit outlet recursively call the function

    if (this->units[unit_num].conc_num < this->units.size()) {
        mark_units(this->units[unit_num].conc_num);
    }
    else {
        validation[0] = true;
        return;
        // ...Potentially do something to indicate that you have seen an exit
    }
    //If tails_num does not point at a circuit outlet recursively call the function

    if (this->units[unit_num].tails_num < this->units.size()) {
        mark_units(this->units[unit_num].tails_num);
    }
    else {
        validation[1] = true;
        return;
        // ...Potentially do something to indicate that you have seen an exit
    }
    return;
}