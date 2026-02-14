cmake_minimum_required(VERSION 3.15)
project(DesignPatternsInModernCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Creational Patterns
add_executable(singleton_example examples/creational/singleton_example.cpp)

# SOLID Principles
add_executable(srp_example 
    src/solid/srp.cpp 
    examples/solid/srp_example.cpp
)

add_executable(ocp_example 
    src/solid/ocp.cpp 
    examples/solid/ocp_example.cpp
)

add_executable(lsp_example 
    src/solid/lsp.cpp 
    examples/solid/lsp_example.cpp
)
