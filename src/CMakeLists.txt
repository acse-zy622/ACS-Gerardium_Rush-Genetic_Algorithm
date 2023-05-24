## add the genetic algorithm library

add_library(geneticAlgorithm Genetic_Algorithm.cpp)

set_target_properties( geneticAlgorithm
    PROPERTIES
    CXX_STANDARD 17
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# build the circuit simulator as a testable library

add_library(circuitSimulator CCircuit.cpp CSimulator.cpp CUint.cpp)
set_target_properties( circuitSimulator
    PROPERTIES
    CXX_STANDARD 17
)

# build the executable

add_executable(Circuit_Optimizer main.cpp)
target_link_libraries(Circuit_Optimizer PUBLIC geneticAlgorithm circuitSimulator)

set_target_properties( Circuit_Optimizer
    PROPERTIES
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
