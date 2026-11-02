
#include <gtest/gtest.h>
#include <stdexcept>

#include "combinatorics.h"
#include "CombinatoricsCalculator.h"

TEST(CombinatoricsTests, Factorial)
{
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(5), 120);
    EXPECT_EQ(factorial(10), 3628800);
}

TEST(CombinatoricsTests, Permutations)
{
    EXPECT_EQ(permutations(4), 24);
    EXPECT_EQ(permutations(5), 120);
}

TEST(CombinatoricsTests, Combinations)
{
    EXPECT_EQ(combinations(5, 2), 10);
    EXPECT_EQ(combinations(6, 3), 20);
    EXPECT_EQ(combinations(10, 0), 1);
}

TEST(CombinatoricsTests, Arrangements)
{
    EXPECT_EQ(arrangements(5, 2), 20);
    EXPECT_EQ(arrangements(4, 2), 12);
}

TEST(CombinatoricsTests, NegativeFactorial)
{
    EXPECT_THROW(factorial(-1), std::invalid_argument);
}

TEST(CombinatoricsTests, NegativePermutations)
{
    EXPECT_THROW(permutations(-1), std::invalid_argument);
}

TEST(CombinatoricsTests, InvalidCombinations)
{
    EXPECT_THROW(combinations(-1, 2), std::invalid_argument);
    EXPECT_THROW(combinations(5, -1), std::invalid_argument);
    EXPECT_THROW(combinations(5, 6), std::invalid_argument);
}

TEST(CombinatoricsTests, InvalidArrangements)
{
    EXPECT_THROW(arrangements(-1, 2), std::invalid_argument);
    EXPECT_THROW(arrangements(5, -1), std::invalid_argument);
    EXPECT_THROW(arrangements(5, 6), std::invalid_argument);
}

TEST(CombinatoricsTests, FactorialOverflow)
{
    EXPECT_THROW(factorial(21), std::overflow_error);
}

TEST(CombinatoricsTests, CombinationsOverflow)
{
    EXPECT_THROW(combinations(21, 2), std::overflow_error);
}

TEST(CombinatoricsTests, ArrangementsOverflow)
{
    EXPECT_THROW(arrangements(21, 2), std::overflow_error);
}

TEST(CombinatoricsTests, MaximumFactorial)
{
    EXPECT_NO_THROW(factorial(20));
    EXPECT_EQ(factorial(20), 2432902008176640000ULL);
}

TEST(CalculatorTests, InitialState)
{
    CombinatoricsCalculator calculator;

    EXPECT_EQ(calculator.getN(), 0);
    EXPECT_EQ(calculator.getK(), 0);
    EXPECT_EQ(calculator.getFactorial(), 1);
    EXPECT_EQ(calculator.getPermutations(), 1);
    EXPECT_EQ(calculator.getCombinations(), 1);
    EXPECT_EQ(calculator.getArrangements(), 1);
}

TEST(CalculatorTests, SuccessfulCalculation)
{
    CombinatoricsCalculator calculator;

    calculator.calculate(5, 2);

    EXPECT_EQ(calculator.getN(), 5);
    EXPECT_EQ(calculator.getK(), 2);
    EXPECT_EQ(calculator.getFactorial(), 120);
    EXPECT_EQ(calculator.getPermutations(), 120);
    EXPECT_EQ(calculator.getCombinations(), 10);
    EXPECT_EQ(calculator.getArrangements(), 20);
}

TEST(CalculatorTests, StatePreservedAfterInvalidArguments)
{
    CombinatoricsCalculator calculator;

    calculator.calculate(5, 2);

    EXPECT_THROW(calculator.calculate(5, 6), std::invalid_argument);

    EXPECT_EQ(calculator.getN(), 5);
    EXPECT_EQ(calculator.getK(), 2);
    EXPECT_EQ(calculator.getFactorial(), 120);
    EXPECT_EQ(calculator.getPermutations(), 120);
    EXPECT_EQ(calculator.getCombinations(), 10);
    EXPECT_EQ(calculator.getArrangements(), 20);
}

TEST(CalculatorTests, StatePreservedAfterOverflow)
{
    CombinatoricsCalculator calculator;

    calculator.calculate(5, 2);

    EXPECT_THROW(calculator.calculate(21, 2), std::overflow_error);

    EXPECT_EQ(calculator.getN(), 5);
    EXPECT_EQ(calculator.getK(), 2);
    EXPECT_EQ(calculator.getFactorial(), 120);
    EXPECT_EQ(calculator.getPermutations(), 120);
    EXPECT_EQ(calculator.getCombinations(), 10);
    EXPECT_EQ(calculator.getArrangements(), 20);
}
