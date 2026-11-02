#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Lab7_WeightedGraph.h"
#include "ScopeLogger.h"

void level3()
{
    ScopeLogger logger("level3");

    std::ofstream file("raii_demo.txt");  

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file");
    }

    file << "RAII demo: stream closed during unwinding\n";

    std::cout << "level3: file opened, adding an edge with a negative weight...\n";

    WeightedGraph graph(3);
    graph.addEdge(0, 1, -5);   

    std::cout << "level3: this line is never reached\n";
}

void level2()
{
    ScopeLogger logger("level2");

    try
    {
        level3();
    }
    catch (const InvalidWeightException& e)
    {
        std::cout << "[level2] caught InvalidWeightException: parameter="
                  << e.parameter() << ", value=" << e.value()
                  << ", minAllowed=" << e.minAllowed() << '\n';

        throw;   
    }
}

void level1()
{
    ScopeLogger logger("level1");
    level2();
}

int main()
{
    std::cout << "RAII AND EXCEPTIONS DEMONSTRATION\n";

    try
    {
        level1();
    }
    catch (const InvalidWeightException& e)
    {
        std::cout << "[main] caught InvalidWeightException\n";
        std::cout << "[main] what(): " << e.what() << '\n';
    }
    catch (const GraphException& e)
    {
        std::cout << "[main] caught GraphException: " << e.what() << '\n';
    }
    catch (const std::exception& e)
    {
        std::cout << "[main] caught std::exception: " << e.what() << '\n';
    }

    std::ifstream in("raii_demo.txt");
    std::string line;
    std::getline(in, line);
    std::cout << "File content after unwinding: " << line << '\n';

    std::cout << "Program continues normally.\n";
    std::cout << "DEMONSTRATION FINISHED\n";

    return 0;
}
