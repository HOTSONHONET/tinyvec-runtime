CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude

BUILD_DIR := build
BIN_DIR := bin

CORE_OBJS := $(BUILD_DIR)/vector.o $(BUILD_DIR)/index.o
CORE_PIC_OBJS := $(BUILD_DIR)/vector.pic.o $(BUILD_DIR)/index.pic.o

STATIC_BIN := $(BIN_DIR)/tinyvec_static
SHARED_BIN := $(BIN_DIR)/tinyvec_shared
PLUGIN_BIN := $(BIN_DIR)/tinyvec_plugin_app

.PHONY: all clean run-static run-shared run-plugin inspect

all: $(STATIC_BIN) $(SHARED_BIN) $(PLUGIN_BIN) $(BUILD_DIR)/libcosine_metric.so

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR)/vector.o: src/vector.cpp include/tinyvec/vector.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c src/vector.cpp -o $@

$(BUILD_DIR)/index.o: src/index.cpp include/tinyvec/index.h include/tinyvec/vector.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c src/index.cpp -o $@

$(BUILD_DIR)/vector.pic.o: src/vector.cpp include/tinyvec/vector.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c src/vector.cpp -o $@

$(BUILD_DIR)/index.pic.o: src/index.cpp include/tinyvec/index.h include/tinyvec/vector.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c src/index.cpp -o $@

$(BUILD_DIR)/main.o: apps/main.cpp include/tinyvec/index.h include/tinyvec/vector.h include/tinyvec/metric_plugin.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c apps/main.cpp -o $@

$(BUILD_DIR)/libtinyvec.a: $(CORE_OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/libtinyvec.so: $(CORE_PIC_OBJS)
	$(CXX) -shared $^ -o $@

$(BUILD_DIR)/libcosine_metric.so: plugins/cosine_metric.cpp include/tinyvec/metric_plugin.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -shared plugins/cosine_metric.cpp -o $@

$(STATIC_BIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/libtinyvec.a | $(BIN_DIR)
	$(CXX) $(BUILD_DIR)/main.o $(BUILD_DIR)/libtinyvec.a -ldl -o $@

$(SHARED_BIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/libtinyvec.so | $(BIN_DIR)
	$(CXX) $(BUILD_DIR)/main.o -L$(BUILD_DIR) -ltinyvec -ldl -o $@

$(PLUGIN_BIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/libtinyvec.so $(BUILD_DIR)/libcosine_metric.so | $(BIN_DIR)
	$(CXX) $(BUILD_DIR)/main.o -L$(BUILD_DIR) -ltinyvec -ldl -o $@

run-static: $(STATIC_BIN) $(BUILD_DIR)/libcosine_metric.so
	$(STATIC_BIN)

run-shared: $(SHARED_BIN) $(BUILD_DIR)/libcosine_metric.so
	LD_LIBRARY_PATH=$(BUILD_DIR) $(SHARED_BIN)

run-plugin: $(PLUGIN_BIN) $(BUILD_DIR)/libcosine_metric.so
	LD_LIBRARY_PATH=$(BUILD_DIR) $(PLUGIN_BIN)

inspect: $(PLUGIN_BIN) $(BUILD_DIR)/libcosine_metric.so
	@echo "Shared library dependencies:"
	LD_LIBRARY_PATH=$(BUILD_DIR) ldd $(PLUGIN_BIN)
	@echo ""
	@echo "Symbols in plugin:"
	nm -D -C $(BUILD_DIR)/libcosine_metric.so | grep create_plugin || true

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)