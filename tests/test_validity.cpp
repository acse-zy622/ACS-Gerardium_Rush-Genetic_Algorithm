#include <iostream>
#include "CUnit.h"
#include "CCircuit.h"

/**
@brief To test the validity function and Circuit class
**/


int main(int argc, char * argv[])
{
    std::cout << "Starting first test on validity" << std::endl;
    //creating an object of the Circuit class
    CCircuit c1(4);
    //initialising the circuit vector
    int circ1[10] = {0,1,3,2,0,4,3,0,5};
    bool val1 = c1.Check_Validity(circ1);
    if (val1){
        std::cout << "First test passed" << std::endl;
    }
    else
    {
        std::cout << "Tests Failed" << std::endl;
        return 1;
    }
    std::cout << "Starting second test on validity" << std::endl;
    //creating an object of the Circuit class
    CCircuit c2(3);
    //initialising the circuit vector
    int circ2[7] = { 0,1,2,3,0,0,4 };
    bool val2 = c2.Check_Validity(circ2);
    if (val2)
    {
        std::cout << "Second test passed" << std::endl;
    }
    else
    {
        std::cout << "Tests Failed" << std::endl;
        return 1;
    }
    std::cout << "All tests passed!" << std::endl;

    return 0;
}
