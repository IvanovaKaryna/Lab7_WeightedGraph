#include "Lab7_WeightedGraph.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace std;

int main()
{
    cout << "Weighted graph testing" << endl;

    WeightedGraph graph(5);

    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 2);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 4, 10);
    graph.addEdge(3, 4, 2);

    cout << "\nTest 1. Shortest path" << endl;

    try
    {
        cout << "Shortest path length: " << graph.shortestPath(0, 4) << endl;

        vector<int> path = graph.reconstructPath(0, 4);

        cout << "Path: ";
        for (int vertex : path)
        {
            cout << vertex << " ";
        }
        cout << endl;
        cout << "Path length: " << graph.pathLength(path) << endl;
    }
    catch (const exception& e)
    {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\nTest 2. Invalid vertex" << endl;

    try
    {
        graph.addEdge(10, 1, 5);
        cout << "Test failed" << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Test passed: " << e.what() << endl;
    }

    cout << "\nTest 3. Negative weight" << endl;

    try
    {
        graph.addEdge(0, 1, -5);
        cout << "Test failed" << endl;
    }
    catch (const InvalidWeightException& e)
    {
        cout << "Test passed: " << e.what() << endl;
        cout << "Parameter: " << e.parameter() << ", value: " << e.value()
             << ", minimum: " << e.minAllowed() << endl;
    }

    cout << "\nTest 4. No path" << endl;

    try
    {
        WeightedGraph disconnected(3);
        disconnected.addEdge(0, 1, 5);
        disconnected.shortestPath(0, 2);
        cout << "Test failed" << endl;
    }
    catch (const PathNotFoundException& e)
    {
        cout << "Test passed: " << e.what() << endl;
    }

    cout << "\nTest 5. Invalid path reconstruction" << endl;

    try
    {
        graph.reconstructPath(0, 10);
        cout << "Test failed" << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Test passed: " << e.what() << endl;
    }

    cout << "\nTest 6. Invalid vertex in path" << endl;

    try
    {
        graph.pathLength({ 0, 1, 10 });
        cout << "Test failed" << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Test passed: " << e.what() << endl;
    }

    cout << "\nTest 7. Path length overflow" << endl;

    try
    {
        WeightedGraph overflowGraph(3);
        overflowGraph.addEdge(0, 1, numeric_limits<int>::max() - 1);
        overflowGraph.addEdge(1, 2, 10);
        overflowGraph.shortestPath(0, 2);
        cout << "Test failed" << endl;
    }
    catch (const overflow_error& e)
    {
        cout << "Test passed: " << e.what() << endl;
    }

    cout << "\nTest 8. Strong exception guarantee" << endl;

    try
    {
        vector<WeightedGraph::Edge> invalidEdges =
        {
            { 0, 1, 7 },
            { 1, 10, 3 }
        };

        graph.replaceEdges(invalidEdges);
        cout << "Test failed" << endl;
    }
    catch (const out_of_range& e)
    {
        cout << "Test passed: " << e.what() << endl;

        try
        {
            int distance = graph.shortestPath(0, 4);

            if (distance == 10)
            {
                cout << "Old graph was preserved. Strong guarantee passed." << endl;
            }
            else
            {
                cout << "Strong guarantee failed." << endl;
            }
        }
        catch (const exception& error)
        {
            cout << "Verification error: " << error.what() << endl;
        }
    }

    cout << "\nTesting finished" << endl;

    return 0;
}
