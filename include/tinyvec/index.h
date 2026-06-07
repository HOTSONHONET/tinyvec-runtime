#pragma once

#include "tinyvec/vector.h"

#include <vector>
#include <string>

namespace tinyvec
{
    class Index
    {
    public:
        void add(Vector vector);
        const Vector *find_by_id(const std::string &id) const;
        const std::vector<Vector> &all() const;

    private:
        std::vector<Vector> vectors_;
    };
}