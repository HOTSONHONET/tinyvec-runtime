#pragma once

#include <cstddef>

extern "C"
{
    struct MetricPlugin
    {
        const char *name;
        float (*score)(const float *a, const float *b, std::size_t n);
    };

    MetricPlugin *create_plugin();
}