#include "tinyvec/vector.h"

#include <iostream>

namespace tinyvec
{
    Vector::Vector(std::string id, std::vector<float> values) : id_(std::move(id)), values_(std::move(values)) {}

    const std::string &Vector::id() const
    {
        return id_;
    }

    const std::vector<float> &Vector::values() const
    {
        return values_;
    }

    void Vector::print() const
    {
        std::cout << id_ << " -> [";

        for (size_t i = 0; i < values_.size(); i++)
        {
            std::cout << values_[i];
            if (i + 1 < values_.size())
            {
                std::cout << ", ";
            }
        }

        std::cout << "]\n";
    }
}