/** Header for the circuit class
 *
 * This header defines the circuit class and its associated functions
 *
*/

#pragma once

#include "CUnit.h"

#include <vector>

class Circuit {
  public:
    Circuit(int num_units);
    bool Check_Validity(int *circuit_vector);
  private:
    void mark_units(int unit_num);
    std::vector<CUnit> units;
};

