#pragma once

#include <vector>

class CombinatoricsCalculator
{
private:
    int n;
    int k;

    unsigned long long factorialResult;
    unsigned long long permutationsResult;
    unsigned long long combinationsResult;
    unsigned long long arrangementsResult;

public:
    CombinatoricsCalculator();

    void calculate(int newN, int newK);

    int getN() const;
    int getK() const;

    unsigned long long getFactorial() const;
    unsigned long long getPermutations() const;
    unsigned long long getCombinations() const;
    unsigned long long getArrangements() const;
};
