
#include "CombinatoricsCalculator.h"
#include "combinatorics.h"

CombinatoricsCalculator::CombinatoricsCalculator()
    : n(0),
    k(0),
    factorialResult(1),
    permutationsResult(1),
    combinationsResult(1),
    arrangementsResult(1)
{
}

void CombinatoricsCalculator::calculate(int newN, int newK)
{
    unsigned long long newFactorial = factorial(newN);
    unsigned long long newPermutations = permutations(newN);
    unsigned long long newCombinations = combinations(newN, newK);
    unsigned long long newArrangements = arrangements(newN, newK);

    n = newN;
    k = newK;

    factorialResult = newFactorial;
    permutationsResult = newPermutations;
    combinationsResult = newCombinations;
    arrangementsResult = newArrangements;
}

int CombinatoricsCalculator::getN() const
{
    return n;
}

int CombinatoricsCalculator::getK() const
{
    return k;
}

unsigned long long CombinatoricsCalculator::getFactorial() const
{
    return factorialResult;
}

unsigned long long CombinatoricsCalculator::getPermutations() const
{
    return permutationsResult;
}

unsigned long long CombinatoricsCalculator::getCombinations() const
{
    return combinationsResult;
}

unsigned long long CombinatoricsCalculator::getArrangements() const
{
    return arrangementsResult;
}
