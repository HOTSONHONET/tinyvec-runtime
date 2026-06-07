#include "tinyvec/index.h"
#include "tinyvec/metric_plugin.h"

#include <iostream>

#include <dlfcn.h>
#include <limits>

using CreatePluginFn = MetricPlugin *(*)();

int main(int argc, char **argv)
{

    const char *plugin_path = "./build/libcosine_metric.so";

    if (argc >= 2)
    {
        plugin_path = argv[1];
    }
    // Load the plugin
    void *handle = dlopen(plugin_path, RTLD_NOW);

    if (handle == nullptr)
    {
        std::cerr << "Failed to load plugin: " << dlerror() << "\n";
        return 1;
    }

    dlerror(); // Clear any existing error

    auto create_plugin = reinterpret_cast<CreatePluginFn>(dlsym(handle, "create_plugin"));

    const char *error = dlerror();
    if (error != nullptr)
    {
        std::cerr << "Failed to find create_plugin: " << error << "\n";
        dlclose(handle);
        return 1;
    }

    MetricPlugin *metric = create_plugin();

    std::cout << "Loaded metric: " << metric->name << "\n";

    // Creating new index
    tinyvec::Index index;

    // Adding new vectors
    index.add(tinyvec::Vector("doc1", {1.0f, 0.0f, 0.0f}));
    index.add(tinyvec::Vector("doc2", {0.0f, 1.0f, 0.0f}));
    index.add(tinyvec::Vector("doc3", {0.9f, 0.1f, 0.0f}));

    std::vector<float> query = {1.0f, 0.0f, 0.0f};

    const tinyvec::Vector *best = nullptr;
    float best_score = -std::numeric_limits<float>::infinity();

    // printing all vectors
    for (const auto &vector : index.all())
    {

        float score = metric->score(
            query.data(),
            vector.values().data(),
            query.size());

        std::cout << "score(" << vector.id() << ") = " << score << "\n";

        if (score > best_score)
        {
            best_score = score;
            best = &vector;
        }
        vector.print();
    }

    if (best != nullptr)
    {
        std::cout << "Best match: ";
        best->print();
    }

    // checking find by id works
    const tinyvec::Vector *found = index.find_by_id("doc3");
    if (found != nullptr)
    {
        std::cout << "Found ";
        found->print();
    }

    dlclose(handle);

    return 0;
}