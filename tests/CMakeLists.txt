project(tests)

list(APPEND Tests test1 test2 test_GA)

foreach(TEST IN LISTS Tests)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} geneticAlgorithm circuitSimulator)
    target_include_directories(${TEST} PRIVATE ../includes)
    set_target_properties(${TEST} PROPERTIES
        CXX_STANDARD 17
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/bin")
    add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

add_test(NAME executable COMMAND "${CMAKE_BINARY_DIR}/bin/Circuit_Optimizer")