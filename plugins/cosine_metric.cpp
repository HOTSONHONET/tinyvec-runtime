#include "tinyvec/metric_plugin.h"

#include <cmath>

static float cosine_score(const float *a, const float *b, std::size_t n)
{
    float dot = 0.0f;
    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (std::size_t i = 0; i < n; i++)
    {
        dot += a[i] * b[i];

        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }

    if (norm_a == 0.0f || norm_b == 0.0f)
    {
        return 0.0f; // floating point division by zero, return 0 similarity
    }

    return dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
}

extern "C" MetricPlugin *create_plugin()
{
    static MetricPlugin plugin{"cosine", cosine_score};
    return &plugin;
}