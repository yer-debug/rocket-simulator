cmake_minimum_required(VERSION 3.16)
project(starship_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -O2)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(starship_sim
    src/main.cpp
    src/Rocket.cpp
    src/Simulator.cpp
    src/Logger.cpp
)

# tiny tests executable (simple run)
add_executable(starship_test
    tests/test_sim.cpp
    src/Rocket.cpp
    src/Simulator.cpp
    src/Logger.cpp
)
