#pragma once

#include <stdexcept>
#include <string>

class GraphException : public std::runtime_error
{
public:
    explicit GraphException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

class InvalidWeightException : public GraphException
{
    std::string parameter_;
    int value_;
    int minAllowed_;

public:
    InvalidWeightException(const std::string& parameter,
                           int value, int minAllowed = 0)
        : GraphException("Invalid weight: parameter '" + parameter + "' = "
            + std::to_string(value) + ", minimum allowed = "
            + std::to_string(minAllowed)),
          parameter_(parameter), value_(value), minAllowed_(minAllowed)
    {
    }

    const std::string& parameter() const noexcept { return parameter_; }
    int value() const noexcept { return value_; }
    int minAllowed() const noexcept { return minAllowed_; }
};

class PathNotFoundException : public GraphException
{
    int start_;
    int finish_;

public:
    PathNotFoundException(int start, int finish)
        : GraphException("Path not found: from vertex " + std::to_string(start)
            + " to vertex " + std::to_string(finish)),
          start_(start), finish_(finish)
    {
    }

    int start() const noexcept { return start_; }
    int finish() const noexcept { return finish_; }
};
