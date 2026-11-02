#include "combinatorics.h"

#include <stdexcept>
#include <limits>

unsigned long long factorial(int n)
{
    if (n < 0)
        throw std::invalid_argument("Factorial: n cannot be negative");

    if (n > 20)
        throw std::overflow_error("Factorial: result is too large");

    unsigned long long result = 1;

    for (int i = 1; i <= n; i++)
    {
        if (result > std::numeric_limits<unsigned long long>::max() / i)
            throw std::overflow_error("Factorial: integer overflow");

        result *= i;
    }

    return result;
}

unsigned long long permutations(int n)
{
    return factorial(n);
}

unsigned long long combinations(int n, int k)
{
    if (n < 0 || k < 0 || k > n)
        throw std::invalid_argument("Combinations: invalid arguments");

    if (n > 20)
        throw std::overflow_error("Combinations: n is too large");

    return factorial(n) / (factorial(k) * factorial(n - k));
}

unsigned long long arrangements(int n, int k)
{
    if (n < 0 || k < 0 || k > n)
        throw std::invalid_argument("Arrangements: invalid arguments");

    if (n > 20)
        throw std::overflow_error("Arrangements: n is too large");

    return factorial(n) / factorial(n - k);
}