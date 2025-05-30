# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O3
CMAKE_BUILD_TYPE ?= Release

# Project configuration
PROJECT_NAME = EfficientMergeAlgorithms

# Directories
SRC_DIR = src
BUILD_DIR = build
TOOLS_DIR = tools
INCLUDE_DIR = include
FRAMEWORK_DIR = $(SRC_DIR)/framework
RESULTS_DIR = results
PLOTS_DIR = plots
VENV_DIR = .venv

# Python settings
PYTHON = python3
VENV_PYTHON = $(VENV_DIR)/bin/python3
VENV_PIP = $(VENV_DIR)/bin/pip

# Default value for fixed array size M
M ?= 1000

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/framework/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.hpp) $(wildcard $(SRC_DIR)/framework/*.hpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(FRAMEWORK_DIR)

# List all PHONY targets
.PHONY: all clean plots_3d plots_2d plots_2d_time generate_data setup venv

# Default target
all: generate_data plots_3d plots_2d plots_2d_time

# Create build directories
$(BUILD_DIR)/framework:
	@mkdir -p $(BUILD_DIR)/framework

$(RESULTS_DIR):
	@mkdir -p $(RESULTS_DIR)

$(PLOTS_DIR):
	@mkdir -p $(PLOTS_DIR)

$(BUILD_DIR)/main: $(OBJECTS) | $(BUILD_DIR)/framework
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)/framework
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Python virtual environment
$(VENV_DIR):
	$(PYTHON) -m venv $(VENV_DIR)

venv: $(VENV_DIR)

# Python dependencies setup
setup: venv
	$(VENV_PIP) install -r requirements.txt

# Data generation target
generate_data: $(BUILD_DIR)/main $(RESULTS_DIR)
	@mkdir -p $(BUILD_DIR)
	./$(BUILD_DIR)/main --csv $(RESULTS_DIR)

# 3D plot generation target
plots_3d: setup $(PLOTS_DIR)
	$(VENV_PYTHON) $(TOOLS_DIR)/plot_generator.py

# 2D plot generation target for comparisons (without data generation)
plots_2d: setup $(PLOTS_DIR)
	$(VENV_PYTHON) $(TOOLS_DIR)/plot_2d_generator.py --m $(M) --metric comparisons

# 2D plot generation target for execution time (without data generation)
plots_2d_time: setup $(PLOTS_DIR)
	$(VENV_PYTHON) $(TOOLS_DIR)/plot_2d_generator.py --m $(M) --metric time

# Clean everything
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(RESULTS_DIR)/*
	rm -rf $(PLOTS_DIR)/*
	rm -rf $(VENV_DIR)
	rm -f *.png 