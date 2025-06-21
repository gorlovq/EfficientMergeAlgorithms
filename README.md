# EfficientMergeAlgorithms

## Authors

- **Sergei Gorlov** - [gorlov1207@gmail.com](mailto:gorlov1207@gmail.com)
- **Igor Stikentzin** - [xerminatrix@gmail.com](mailto:xerminatrix@gmail.com)

## Implemented Algorithms

- **Two Way Merge Algorithm**: The traditional method for merging two sorted arrays of lengths `n` and `m`.
- **Binary Insertion for One Element**: An algorithm for inserting a single element into a sorted array using binary search to find the correct position.
- **Hwang-Lin Merging Algorithm** by Donald Knuth: A classic algorithm designed to reduce the number of comparisons during the merge process.
- **Fractile Insertion Algorithm** by Michael Tanner: A stable merge by recursively median-inserting via exponential and binary search.
- **SymMerge Algorithm** by Pok-Son Kim and Arne Kutzner: A sophisticated algorithm focusing on minimizing storage requirements during the merge process.
- **Unstable Core Algorithm** by Pok-Son Kim and Arne Kutzner:In-place $\sqrt(m)$-block rotations with a floating hole, unstable but near-linear moves.

## Building and Running

### Prerequisites

- C++ compiler with C++20 support (g++ or clang++)
- Python 3.8 or higher
- Make

### Setup and Build

1. Clone the repository:
   ```bash
   git clone https://github.com/gorlovq/EfficientMergeAlgorithms.git
   cd EfficientMergeAlgorithms
   ```

2. Build the project:
   ```bash
   make
   ```
   This will:
   - Compile the C++ sources
   - Create a Python virtual environment
   - Install necessary Python dependencies (pandas, matplotlib, scipy)
   - Generate test data
   - Generate all plots

### Generating Only Specific Plots

- To generate only 3D plots:
   ```bash
   make plots_3d
   ```

- To generate 2D comparison plots with a specific M value (default is 1000):
   ```bash
   make plots_2d M=1000
   ```

- To generate 2D execution time plots with a specific M value:
   ```bash
   make plots_2d_time M=1000
   ```

### Regenerating Data

If you need to regenerate the test data:
```bash
make generate_data
```

### Cleaning Up

To clean all generated files:
```bash
make clean
```

## Generated Plots

After running the commands, the plots will be available in the `plots` directory:
- 3D plots: showing each algorithm's performance across different combinations of array sizes
- 2D comparison plots: showing number of comparisons with fixed first array size
- 2D execution time plots: showing execution time with fixed first array size
