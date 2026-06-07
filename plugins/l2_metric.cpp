#include "tinyvec/metric_plugin.h"

#include <cmath>

static float l2_score(const float *a, const float *b, std::size_t n)
{
    float sum = 0.0f;

    for (std::size_t i = 0; i < n; i++)
    {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }

    return -std::sqrt(sum);
}

extern "C" MetricPlugin *create_plugin()
{
    static MetricPlugin plugin{
        "negative_l2",
        l2_score};

    return &plugin;
}