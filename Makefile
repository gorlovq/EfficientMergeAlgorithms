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

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/framework/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.hpp) $(wildcard $(SRC_DIR)/framework/*.hpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(FRAMEWORK_DIR)

# Targets
.PHONY: all clean plots generate_data setup venv

all: generate_data plots

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
	$(VENV_PIP) install pandas matplotlib

# Data generation target
generate_data: $(BUILD_DIR)/main $(RESULTS_DIR)
	@mkdir -p $(BUILD_DIR)
	./$(BUILD_DIR)/main --csv $(RESULTS_DIR)

# Plot generation targets
plots: generate_data setup $(PLOTS_DIR)
	$(VENV_PYTHON) $(TOOLS_DIR)/plot_generator.py

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(RESULTS_DIR)/*
	rm -rf $(PLOTS_DIR)/*
	rm -rf $(VENV_DIR)
	rm -f *.png 