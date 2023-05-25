#include "CCircuit.h"
#include <iostream>


// Default Constructor
CCircuit::CCircuit() : num_units(0), system_mineral_input(0.0), system_waste_input(0.0) {}

/**
* @brief Constructor of the CCircuit class that initialises the vector of 
*        CUnit units
**/

CCircuit::CCircuit(int num_units) {
    this->units.resize(num_units);
    for (int i = 0; i < num_units; i++) {
        this->units[i].conc_num = -1;
        this->units[i].tails_num = -1;
        this->units[i].mark = false;
    }
}

/**
 * @brief Constructor of the CCircuit class with specified number of units and a circuit vector.
 *
 * This constructor initializes a CCircuit object with the given number of units and circuit vector.
 *
 * @param num_units_ The number of units in the circuit.
 * @param CCircuit_vector_ The circuit vector containing information about the circuit.
 */
CCircuit::CCircuit(int num_units_, const std::vector<int>& CCircuit_vector_) : num_units(num_units_), system_mineral_input(0.0), system_waste_input(0.0),
                                                                               CCircuit_vector(CCircuit_vector_) {}

/**
 * @brief Calculates and returns the profit based on the current unit's data.
 *
 * This function calculates the profit based on the current unit's data and returns the result.
 *
 * @return The calculated profit based on the formula: ProfitGerardium * Fgi - PenalizationWaste * Fwi.
 */

double CCircuit::ReturnProfit() {
    double ProfitGerardium = 100;
    double PenalizationWaste = 500;
    return ProfitGerardium * units[num_units].Fgi - PenalizationWaste * units[num_units].Fwi;
}



/**
 * @brief Sets the system flow rates for mineral and waste inputs.
 *
 * This function sets the system flow rates for mineral and waste inputs based on the provided values.
 *
 * @param Sg_ The mineral input flow rate to be set for the system.
 * @param Sw_ The waste input flow rate to be set for the system.
 */
void CCircuit::SystemFlowrates(double Sg_, double Sw_) {
    system_mineral_input = Sg_;
    system_waste_input = Sw_;
}

/**
 * @brief Resets the flow rates of all units in the circuit to zero.
 *
 * This function sets the flow rates of all units in the circuit to zero. It iterates through each unit
 * and resets various flow rate variables to zero, including Fti, Fgi, Fwi, Ftc, Fgc, Fwc, Ftt, Fgt, Fwt,
 * Rg, Rw, and tau.
 */
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

/**
 * @brief Sets the inlet flow rates for each unit in the circuit.
 *
 * This function sets the inlet flow rates for each unit in the circuit based on the system feed ID and
 * the provided recycle mineral and waste vectors. It iterates through each unit and checks if the unit's
 * index matches the system feed ID. If a match is found, the mineral and waste input flow rates (Fgi and Fwi)
 * as well as the total input flow rate (Fti) are set to the sum of the corresponding recycle flow rates,
 * system mineral input, and system waste input. For units that don't match the system feed ID, the inlet
 * flow rates are set to the corresponding recycle flow rates.
 *
 * @note The function assumes that the `CCircuit_vector`, `recycle_mineral`, and `recycle_waste` vectors
 *       have been properly initialized before calling this function.
 */
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

/**
 * @brief Returns the mineral and waste flow rates of each unit in the circuit.
 *
 * This function populates the provided vectors with the mineral and waste flow rates of each unit
 * in the circuit. The function resizes the `Flowrates_g` and `Flowrates_w` vectors to accommodate the
 * required number of flow rates. Then, it iterates through each unit and assigns the mineral and waste
 * flow rates to the corresponding elements in the vectors.
 *
 * @param[out] Flowrates_g A vector that will be filled with the mineral flow rates of each unit.
 * @param[out] Flowrates_w A vector that will be filled with the waste flow rates of each unit.
 *
 * @note The function assumes that the `Flowrates_g` and `Flowrates_w` vectors have been properly
 *       initialized before calling this function.
 */
void CCircuit::ReturnFlowrates(std::vector<double>& Flowrates_g, std::vector<double>& Flowrates_w) {
    Flowrates_g.resize(num_units + 2);
    Flowrates_w.resize(num_units + 2);
    for (int i = 0; i < num_units + 2; i++) {
        Flowrates_g[i] = units[i].Fgi;
        Flowrates_w[i] = units[i].Fwi;
    }
}

/**

*@brief Solves the CCircuit by iterating through its units.

*This function solves the CCircuit by performing the following steps:
*Resets the flow rates.
*Sets the inlet.
*Initializes the recycle_mineral and recycle_waste vectors with zeros.
*Iterates through each unit in the circuit and solves it using SolveCUnit().
*Updates the recycle_mineral and recycle_waste vectors based on the unit outputs.
*@note This function assumes that the CCircuit and its units have been properly initialized.

*@see CCircuit, SolveCUnit()
*/
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

/**
*
*@brief Fills the IDs of the CCircuit units and initializes related data structures.
*This function fills the IDs of the CCircuit units and performs the following steps:
*Resizes the units, recycle_mineral, and recycle_waste vectors based on the number of units.
*Initializes the recycle_mineral and recycle_waste vectors with zeros.
*Assigns the appropriate IDs to each unit based on the CCircuit_vector.
*Sets the IDs for the concentrate and tailing outlets of the system.

*@note This function assumes that the CCircuit, units, and related data structures have been properly initialized.

*@see CCircuit

*@param CCircuit_vector A vector containing the IDs of the units in the CCircuit.
*/

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

bool validation[2] = { false, false };
/**
* @brief Function to check the validity of a circuit 
* @param int * circuit_vector
* Pointer to a circuit vector that contains information about the circuit
* The function iterates through circuit to check its validity
**/
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
/**
*@brief Function to mark all units accessible from the feed
*@param int unit_num 
*Integer value representing the initial unit
*This function is recursively called to mark all units that can be accessed from the feed
**/

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
        // Indicates that you have seen an exit
    }
    //If tails_num does not point at a circuit outlet recursively call the function

    if (this->units[unit_num].tails_num < this->units.size()) {
        mark_units(this->units[unit_num].tails_num);
    }
    else {
        validation[1] = true;
        return;
        // Indicates that you have seen an exit
    }
    return;
}
