#include "Lab7_WeightedGraph.h"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <vector>

using namespace std;

namespace
{
    // Допоміжна функція: граф із 5 вершин, найкоротший шлях 0 -> 4 дорівнює 10.
    WeightedGraph makeSampleGraph()
    {
        WeightedGraph graph(5);
        graph.addEdge(0, 1, 4);
        graph.addEdge(0, 2, 2);
        graph.addEdge(1, 2, 1);
        graph.addEdge(1, 3, 5);
        graph.addEdge(2, 3, 8);
        graph.addEdge(2, 4, 10);
        graph.addEdge(3, 4, 2);
        return graph;
    }
}

// ---------- Основні операції та помилки ----------

TEST(WeightedGraphTests, CreateGraph)
{
    EXPECT_NO_THROW(WeightedGraph graph(5));
}

TEST(WeightedGraphTests, InvalidVertexCount)
{
    EXPECT_THROW(WeightedGraph graph(0), invalid_argument);
    EXPECT_THROW(WeightedGraph graph(-3), invalid_argument);
}

TEST(WeightedGraphTests, ShortestPath)
{
    WeightedGraph graph(5);

    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 2);
    graph.addEdge(2, 1, 1);
    graph.addEdge(1, 3, 5);
    graph.addEdge(3, 4, 3);

    EXPECT_EQ(graph.shortestPath(0, 4), 11);
}

TEST(WeightedGraphTests, ShortestPathChoosesCheaperRoute)
{
    WeightedGraph graph = makeSampleGraph();

    EXPECT_EQ(graph.shortestPath(0, 4), 10);

    vector<int> expected = { 0, 2, 1, 3, 4 };
    vector<int> path = graph.reconstructPath(0, 4);

    EXPECT_EQ(path, expected);
    EXPECT_EQ(graph.pathLength(path), 10);
}

TEST(WeightedGraphTests, InvalidVertex)
{
    WeightedGraph graph(3);

    EXPECT_THROW(graph.addEdge(0, 5, 2), out_of_range);
    EXPECT_THROW(graph.shortestPath(0, 5), out_of_range);
}

TEST(WeightedGraphTests, NegativeWeight)
{
    WeightedGraph graph(3);

    EXPECT_THROW(graph.addEdge(0, 1, -5), InvalidWeightException);
}

TEST(WeightedGraphTests, NegativeWeightDiagnosticData)
{
    WeightedGraph graph(3);

    try
    {
        graph.addEdge(0, 1, -5);
        FAIL() << "Exception was expected";
    }
    catch (const InvalidWeightException& e)
    {
        EXPECT_EQ(e.parameter(), "weight");
        EXPECT_EQ(e.value(), -5);
        EXPECT_EQ(e.minAllowed(), 0);
    }
}

TEST(WeightedGraphTests, NoPath)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 5);

    EXPECT_THROW(graph.shortestPath(0, 2), PathNotFoundException);
    EXPECT_THROW(graph.reconstructPath(0, 2), PathNotFoundException);
}

TEST(WeightedGraphTests, NoPathDiagnosticData)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 5);

    try
    {
        graph.shortestPath(0, 2);
        FAIL() << "Exception was expected";
    }
    catch (const PathNotFoundException& e)
    {
        EXPECT_EQ(e.start(), 0);
        EXPECT_EQ(e.finish(), 2);
    }
}

TEST(WeightedGraphTests, ReconstructPath)
{
    WeightedGraph graph(4);

    graph.addEdge(0, 1, 2);
    graph.addEdge(1, 2, 3);
    graph.addEdge(2, 3, 4);

    vector<int> expected = { 0, 1, 2, 3 };

    EXPECT_EQ(graph.reconstructPath(0, 3), expected);
}

TEST(WeightedGraphTests, InvalidReconstructionIndex)
{
    WeightedGraph graph(3);

    EXPECT_THROW(graph.reconstructPath(0, 5), out_of_range);
}

TEST(WeightedGraphTests, InvalidVertexInPath)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 1);

    vector<int> path = { 0, 1, 5 };

    EXPECT_THROW(graph.pathLength(path), out_of_range);
}

TEST(WeightedGraphTests, InvalidPathArguments)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 1);

    EXPECT_THROW(graph.pathLength({}), invalid_argument);          // порожній маршрут
    EXPECT_THROW(graph.pathLength({ 0, 2 }), invalid_argument);    // немає ребра
}

TEST(WeightedGraphTests, PathLengthOverflow)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, numeric_limits<int>::max() - 1);
    graph.addEdge(1, 2, 10);

    EXPECT_THROW(graph.shortestPath(0, 2), overflow_error);
    EXPECT_THROW(graph.pathLength({ 0, 1, 2 }), overflow_error);
}

// ---------- Ієрархія винятків ----------

TEST(ExceptionHierarchyTests, CustomExceptionsDeriveFromRuntimeError)
{
    EXPECT_THROW(throw InvalidWeightException("weight", -1), GraphException);
    EXPECT_THROW(throw InvalidWeightException("weight", -1), runtime_error);
    EXPECT_THROW(throw PathNotFoundException(0, 1), GraphException);
    EXPECT_THROW(throw PathNotFoundException(0, 1), runtime_error);
    EXPECT_THROW(throw PathNotFoundException(0, 1), exception);
}

// ---------- Граничні значення ----------

TEST(BoundaryTests, ZeroWeightIsAllowed)
{
    WeightedGraph graph(2);

    EXPECT_NO_THROW(graph.addEdge(0, 1, 0));
    EXPECT_EQ(graph.shortestPath(0, 1), 0);
}

TEST(BoundaryTests, MaximumSingleEdgeWeight)
{
    WeightedGraph graph(2);

    EXPECT_NO_THROW(graph.addEdge(0, 1, numeric_limits<int>::max()));
    EXPECT_EQ(graph.shortestPath(0, 1), numeric_limits<int>::max());
}

TEST(BoundaryTests, PathLengthEqualToIntMaxIsNotOverflow)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, numeric_limits<int>::max() - 10);
    graph.addEdge(1, 2, 10);

    EXPECT_NO_THROW(graph.shortestPath(0, 2));
    EXPECT_EQ(graph.shortestPath(0, 2), numeric_limits<int>::max());
    EXPECT_EQ(graph.pathLength({ 0, 1, 2 }), numeric_limits<int>::max());
}

TEST(BoundaryTests, SingleVertexGraph)
{
    WeightedGraph graph(1);

    vector<int> expected = { 0 };

    EXPECT_EQ(graph.shortestPath(0, 0), 0);
    EXPECT_EQ(graph.reconstructPath(0, 0), expected);
}

TEST(BoundaryTests, LastValidVertexIsAccepted)
{
    WeightedGraph graph(3);

    EXPECT_NO_THROW(graph.addEdge(0, 2, 1));
    EXPECT_THROW(graph.addEdge(0, 3, 1), out_of_range);
}

// ---------- Інваріанти ----------

TEST(InvariantTests, VertexCountAndSymmetry)
{
    WeightedGraph graph(4);

    EXPECT_EQ(graph.vertices(), 4);

    graph.addEdge(1, 3, 7);

    EXPECT_TRUE(graph.hasEdge(1, 3));
    EXPECT_TRUE(graph.hasEdge(3, 1));
    EXPECT_FALSE(graph.hasEdge(0, 2));
}

TEST(InvariantTests, VertexCountUnchangedAfterReplace)
{
    WeightedGraph graph(4);

    EXPECT_NO_THROW(graph.replaceEdges({ { 0, 1, 3 } }));
    EXPECT_EQ(graph.vertices(), 4);

    EXPECT_THROW(graph.replaceEdges({ { 0, 9, 3 } }), out_of_range);
    EXPECT_EQ(graph.vertices(), 4);
}

// ---------- Строга гарантія безпеки ----------

TEST(StrongGuaranteeTests, AddEdgeKeepsStateAfterFailure)
{
    WeightedGraph graph = makeSampleGraph();

    int edgesBefore = graph.edgeCount();

    EXPECT_THROW(graph.addEdge(0, 9, 3), out_of_range);
    EXPECT_THROW(graph.addEdge(0, 1, -3), InvalidWeightException);

    EXPECT_EQ(graph.edgeCount(), edgesBefore);
    EXPECT_EQ(graph.shortestPath(0, 4), 10);
}

TEST(StrongGuaranteeTests, ReplaceEdgesInvalidVertex)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 5);

    vector<WeightedGraph::Edge> newEdges = {
        { 0, 2, 10 },
        { 0, 5, 3 }
    };

    EXPECT_THROW(graph.replaceEdges(newEdges), out_of_range);

    EXPECT_EQ(graph.shortestPath(0, 1), 5);
    EXPECT_THROW(graph.shortestPath(0, 2), PathNotFoundException);
}

TEST(StrongGuaranteeTests, ReplaceEdgesNegativeWeight)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 7);

    vector<WeightedGraph::Edge> newEdges = {
        { 0, 2, 10 },
        { 1, 2, -4 }
    };

    EXPECT_THROW(graph.replaceEdges(newEdges), InvalidWeightException);

    EXPECT_EQ(graph.shortestPath(0, 1), 7);
    EXPECT_THROW(graph.shortestPath(0, 2), PathNotFoundException);
}

TEST(StrongGuaranteeTests, ReplaceEdgesPreservesWholeGraph)
{
    WeightedGraph graph = makeSampleGraph();

    // Знімок стану до невдалої операції.
    vector<vector<bool>> before(5, vector<bool>(5, false));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            before[i][j] = graph.hasEdge(i, j);
    int edgesBefore = graph.edgeCount();

    vector<WeightedGraph::Edge> newEdges = {
        { 0, 4, 1 },
        { 1, 3, 1 },
        { 2, 7, 1 }
    };

    EXPECT_THROW(graph.replaceEdges(newEdges), out_of_range);

    EXPECT_EQ(graph.edgeCount(), edgesBefore);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            EXPECT_EQ(graph.hasEdge(i, j), before[i][j]);

    EXPECT_EQ(graph.shortestPath(0, 4), 10);
}

TEST(StrongGuaranteeTests, ReplaceEdgesSuccess)
{
    WeightedGraph graph(3);

    graph.addEdge(0, 1, 5);

    EXPECT_NO_THROW(graph.replaceEdges({ { 1, 2, 4 } }));

    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 2));
    EXPECT_EQ(graph.shortestPath(1, 2), 4);
}

TEST(StrongGuaranteeTests, ReplaceEdgesWithEmptySetClearsGraph)
{
    WeightedGraph graph = makeSampleGraph();

    EXPECT_NO_THROW(graph.replaceEdges({}));

    EXPECT_EQ(graph.edgeCount(), 0);
    EXPECT_THROW(graph.shortestPath(0, 4), PathNotFoundException);
}
