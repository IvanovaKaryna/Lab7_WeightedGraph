#include "Lab7_WeightedGraph.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>

namespace
{
    constexpr long long INF = std::numeric_limits<long long>::max();
    constexpr long long INT_LIMIT = std::numeric_limits<int>::max();
}

WeightedGraph::WeightedGraph(int vertices) : vertexCount(vertices)
{
    if (vertices <= 0)
    {
        throw std::invalid_argument("Number of vertices must be positive");
    }

    graph.assign(vertices, std::vector<int>(vertices, NO_EDGE));
}

void WeightedGraph::checkVertex(int vertex) const
{
    if (vertex < 0 || vertex >= vertexCount)
    {
        throw std::out_of_range("Vertex " + std::to_string(vertex)
            + " is out of range [0, " + std::to_string(vertexCount - 1) + "]");
    }
}

void WeightedGraph::checkWeight(int weight)
{
    if (weight < 0)
    {
        throw InvalidWeightException("weight", weight, 0);
    }
}

void WeightedGraph::addEdge(int from, int to, int weight)
{
    checkVertex(from);
    checkVertex(to);
    checkWeight(weight);

    graph[from][to] = weight;
    graph[to][from] = weight;
}

void WeightedGraph::replaceEdges(const std::vector<Edge>& edges)
{
    std::vector<std::vector<int>> temp(
        vertexCount, std::vector<int>(vertexCount, NO_EDGE));

    for (const Edge& edge : edges)
    {
        checkVertex(edge.from);
        checkVertex(edge.to);
        checkWeight(edge.weight);

        temp[edge.from][edge.to] = edge.weight;
        temp[edge.to][edge.from] = edge.weight;
    }

    graph.swap(temp);
}

bool WeightedGraph::hasEdge(int from, int to) const
{
    checkVertex(from);
    checkVertex(to);

    return graph[from][to] != NO_EDGE;
}

void WeightedGraph::dijkstra(int start,
                             std::vector<long long>& dist,
                             std::vector<int>& parent) const
{
    dist.assign(vertexCount, INF);
    parent.assign(vertexCount, -1);

    std::vector<bool> visited(vertexCount, false);
    dist[start] = 0;

    for (int i = 0; i < vertexCount; ++i)
    {
        int u = -1;

        for (int v = 0; v < vertexCount; ++v)
        {
            if (!visited[v] && dist[v] != INF && (u == -1 || dist[v] < dist[u]))
            {
                u = v;
            }
        }

        if (u == -1)
        {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < vertexCount; ++v)
        {
            if (graph[u][v] == NO_EDGE || visited[v])
            {
                continue;
            }

            long long candidate = dist[u] + graph[u][v];

            if (candidate < dist[v])
            {
                dist[v] = candidate;
                parent[v] = u;
            }
        }
    }
}

int WeightedGraph::shortestPath(int start, int finish) const
{
    checkVertex(start);
    checkVertex(finish);

    std::vector<long long> dist;
    std::vector<int> parent;
    dijkstra(start, dist, parent);

    if (dist[finish] == INF)
    {
        throw PathNotFoundException(start, finish);
    }

    if (dist[finish] > INT_LIMIT)
    {
        throw std::overflow_error("Path length exceeds INT_MAX");
    }

    return static_cast<int>(dist[finish]);
}

std::vector<int> WeightedGraph::reconstructPath(int start, int finish) const
{
    checkVertex(start);
    checkVertex(finish);

    std::vector<long long> dist;
    std::vector<int> parent;
    dijkstra(start, dist, parent);

    if (dist[finish] == INF)
    {
        throw PathNotFoundException(start, finish);
    }

    std::vector<int> path;

    for (int v = finish; v != -1; v = parent[v])
    {
        path.push_back(v);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

int WeightedGraph::pathLength(const std::vector<int>& path) const
{
    if (path.empty())
    {
        throw std::invalid_argument("Path must not be empty");
    }

    for (int vertex : path)
    {
        checkVertex(vertex);
    }

    long long total = 0;

    for (std::size_t i = 1; i < path.size(); ++i)
    {
        int weight = graph[path[i - 1]][path[i]];

        if (weight == NO_EDGE)
        {
            throw std::invalid_argument("No edge between vertices "
                + std::to_string(path[i - 1]) + " and " + std::to_string(path[i]));
        }

        total += weight;

        if (total > INT_LIMIT)
        {
            throw std::overflow_error("Path length exceeds INT_MAX");
        }
    }

    return static_cast<int>(total);
}

int WeightedGraph::vertices() const noexcept
{
    return vertexCount;
}

int WeightedGraph::edgeCount() const noexcept
{
    int count = 0;

    for (int i = 0; i < vertexCount; ++i)
    {
        for (int j = i; j < vertexCount; ++j)
        {
            if (graph[i][j] != NO_EDGE)
            {
                ++count;
            }
        }
    }

    return count;
}
