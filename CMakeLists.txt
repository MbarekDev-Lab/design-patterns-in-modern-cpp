cmake_minimum_required(VERSION 3.15)
project(DesignPatternsInModernCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Creational Patterns
add_executable(singleton_example examples/creational/singleton_example.cpp)
