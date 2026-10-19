cmake_minimum_required(VERSION 3.10)
project(MatrixBenchmark)

set(CMAKE_CXX_STANDARD 17)

# -O3: Max speed optimization (The "Go Fast" button)
# -march=native: Use your specific CPU's special instructions
set(CMAKE_CXX_FLAGS "-O3 -march=native")

include_directories(include)

add_executable(benchmark src/main.cpp src/matrix.cpp)

enable_testing()

add_executable(matrix_tests tests/test_matrix.cpp src/matrix.cpp)
add_test(NAME matrix_tests COMMAND matrix_tests)
