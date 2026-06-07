#include "tinyvec/index.h"

#include <iostream>

namespace tinyvec
{
    void Index::add(Vector vector)
    {
        vectors_.push_back(std::move(vector));
    }

    const Vector *Index::find_by_id(const std::string &id) const
    {
        for (const auto &vector : vectors_)
        {
            if (vector.id() == id)
            {
                return &vector;
            }
        }

        return nullptr;
    }

    const std::vector<Vector> &Index::all() const
    {
        return vectors_;
    }
}
