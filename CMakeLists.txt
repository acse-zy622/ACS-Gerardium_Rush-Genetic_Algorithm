cmake_minimum_required(VERSION 3.10)

# set the project name
project(Genetic_Algorithm)

# set the include path
include_directories(include)

# add the main code
add_subdirectory(src)

# add the tests
include(CTest)
enable_testing()
add_subdirectory(tests)


