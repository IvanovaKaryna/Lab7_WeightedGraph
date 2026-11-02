#pragma once

#include <iostream>
#include <string>

class ScopeLogger
{
    std::string name_;

public:
    explicit ScopeLogger(const std::string& name) : name_(name)
    {
        std::cout << "[ENTER] " << name_ << '\n';
    }

    ~ScopeLogger() noexcept
    {
        std::cout << "[EXIT ] " << name_ << '\n';
    }

    ScopeLogger(const ScopeLogger&) = delete;
    ScopeLogger& operator=(const ScopeLogger&) = delete;
};
