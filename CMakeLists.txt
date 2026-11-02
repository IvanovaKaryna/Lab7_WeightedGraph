cmake_minimum_required(VERSION 3.20)

project(Lab7_WeightedGraph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Статична бібліотека WeightedGraph
add_library(WeightedGraph STATIC
    WeightedGraph.cpp
    Lab7_WeightedGraph.h
    GraphExceptions.h
)

target_include_directories(WeightedGraph PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Демонстраційна програма
add_executable(Lab7_WeightedGraph
    Lab7_WeightedGraph.cpp
)

target_link_libraries(Lab7_WeightedGraph PRIVATE
    WeightedGraph
)

# Демонстрація RAII, розкручування стека та throw;
add_executable(ExceptionDemo
    ExceptionDemo.cpp
)

target_link_libraries(ExceptionDemo PRIVATE
    WeightedGraph
)

# Демонстрація некоректних підходів (throw int / std::string)
add_executable(AntipatternDemo
    AntipatternDemo.cpp
)

# Google Test через vcpkg
find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(WeightedGraphTests
    WeightedGraphTests.cpp
)

target_link_libraries(WeightedGraphTests PRIVATE
    WeightedGraph
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(WeightedGraphTests)