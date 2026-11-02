#include <exception>
#include <iostream>
#include <string>

void throwInt()
{
    throw 42;
}

void throwString()
{
    throw std::string("error as std::string");
}

int main()
{
    std::cout << "ANTIPATTERN DEMONSTRATION\n";

    // 1. throw int
    try
    {
        try
        {
            throwInt();
        }
        catch (const std::exception& e)
        {
            std::cout << "int caught by std::exception: " << e.what() << '\n';
        }
    }
    catch (...)
    {
        std::cout << "int was NOT caught by catch (const std::exception&), "
                     "but was caught by catch (...)\n";
    }

    // 2. throw std::string
    try
    {
        try
        {
            throwString();
        }
        catch (const std::exception& e)
        {
            std::cout << "string caught by std::exception: " << e.what() << '\n';
        }
    }
    catch (...)
    {
        std::cout << "std::string was NOT caught by catch (const std::exception&), "
                     "but was caught by catch (...)\n";
    }

    std::cout << "catch (...) has no parameter, so we cannot read the error text.\n";
    std::cout << "DEMONSTRATION FINISHED\n";

    return 0;
}
