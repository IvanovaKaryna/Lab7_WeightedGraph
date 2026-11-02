#pragma once

#include <vector>
#include "GraphExceptions.h"

class WeightedGraph
{
public:
    struct Edge
    {
        int from;
        int to;
        int weight;
    };

    static constexpr int NO_EDGE = -1;

private:
    int vertexCount;
    std::vector<std::vector<int>> graph;

    void checkVertex(int vertex) const;      
    static void checkWeight(int weight);   

    void dijkstra(int start,
                  std::vector<long long>& dist,
                  std::vector<int>& parent) const;

public:
    explicit WeightedGraph(int vertices);

    void addEdge(int from, int to, int weight);

    void replaceEdges(const std::vector<Edge>& edges);

    bool hasEdge(int from, int to) const;
    int shortestPath(int start, int finish) const;
    int pathLength(const std::vector<int>& path) const;
    std::vector<int> reconstructPath(int start, int finish) const;

    int vertices() const noexcept;
    int edgeCount() const noexcept;
};
