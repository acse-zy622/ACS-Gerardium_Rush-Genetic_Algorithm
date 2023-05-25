set(CMAKE_BUILD_TYPE Release)
find_package(OpenMP)

## add the genetic algorithm library

add_library(geneticAlgorithm Genetic_Algorithm.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(geneticAlgorithm PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties( geneticAlgorithm
    PROPERTIES
    CXX_STANDARD 17
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# build the circuit simulator as a testable library

add_library(circuitSimulator CCircuit.cpp CSimulator.cpp CUnit.cpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(circuitSimulator PUBLIC OpenMP::OpenMP_CXX)
endif()
set_target_properties( circuitSimulator
    PROPERTIES
    CXX_STANDARD 17
)

# build the hyper search library

add_library(HyperPSearch HyperPSearch.cpp Genetic_Algorithm.cpp CCircuit.cpp CSimulator.cpp CUnit.cpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(HyperPSearch PUBLIC OpenMP::OpenMP_CXX)
endif()
set_target_properties( HyperPSearch
    PROPERTIES
    CXX_STANDARD 17
)

# build the executable

add_executable(Circuit_Optimizer main.cpp)
target_link_libraries(Circuit_Optimizer PUBLIC geneticAlgorithm HyperPSearch circuitSimulator)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Circuit_Optimizer PUBLIC OpenMP::OpenMP_CXX)
endif()
set_target_properties( Circuit_Optimizer
    PROPERTIES
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
