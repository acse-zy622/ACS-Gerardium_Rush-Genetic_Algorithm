#include <cmath>
#include <iostream>

#include "CSimulator.h"

int main(int argc, char * argv[])
{
      int vec1[] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9,
				10, 11, 10, 11, 10, 11, 10, 11}  ;

      int vec2[] = {0, 1, 11, 2, 11, 3, 11, 4, 11, 5, 11, 6, 11,
				7, 11, 8, 11, 9, 11, 10, 11}  ;
      std::cout << "Evaluate_Circuit(21, vec1) close to 42000.0:\n";
      double result = Evaluate_Circuit(21, vec1);
      std::cout << "Evaluate_Circuit(21, vec1) = "<< result <<"\n";
      if (std::fabs(result - 42000.0)<1.0e-8) {
                  std::cout << "pass\n";
            } else {
	        std::cout << "fail\n";
              return 1;
           }

      std::cout << "Evaluate_Circuit(21, vec2) close to 37500.0:\n";
      result =  Evaluate_Circuit(21, vec2);
      std::cout << "Evaluate_Circuit(21, vec2) = "<< result<<"\n";
      if (std::fabs(result - 37500.0)<1.0e-8)
	        std::cout << "pass\n";
      else
            { 
	        std::cout << "fail";
              return 1;
            }
	
}
