#pragma once

#include <vector>
#include <string>

namespace tinyvec
{
    class Vector
    {
    public:
        Vector(std::string id, std::vector<float> values);

        const std::string &id() const;
        const std::vector<float> &values() const;

        void print() const;

    private:
        std::string id_;
        std::vector<float> values_;
    };
}