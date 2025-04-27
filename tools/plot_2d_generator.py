import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob
import argparse
import sys
from matplotlib.ticker import LogLocator, FuncFormatter, ScalarFormatter

# Parse command line arguments
parser = argparse.ArgumentParser(description='Generate 2D comparison plot for merge algorithms')
parser.add_argument('--m', type=int, default=1000, help='Fixed size of the first array (default: 1000)')
parser.add_argument('--metric', type=str, choices=['comparisons', 'time'], default='comparisons', 
                    help='Metric to plot: comparisons or execution time (default: comparisons)')
args = parser.parse_args()

# Get CSV files and check if they exist
csv_files = glob.glob('results/*.csv')

if not csv_files:
    print("Error: No CSV files found in the 'results' directory.")
    sys.exit(1)

# Create output directory
plots_dir = 'plots'
if not os.path.exists(plots_dir):
    os.makedirs(plots_dir)

# Get parameters from command line
FIXED_M = args.m
METRIC = args.metric

# Define colors for algorithms
COLORS = {
    'TwoWayMerge': '#4472C4',  # Blue
    'FractialInsertionMerge': '#ED7D31',  # Orange
    'HwangLinDynamicMerge': '#A5A5A5',  # Gray
    'HwangLinKnuthMerge': '#FFC000',  # Yellow
    'HwangLinStaticMerge': '#5B9BD5',  # Light Blue
}

# Initialize data containers
algorithms = []
sizes_n = []
metrics_values = []
found_m_value = False

# Select metric column based on input
metric_column = 'Comparisons' if METRIC == 'comparisons' else 'Time(ms)'

# Process each algorithm's CSV file
for csv_file in csv_files:
    algorithm_name = os.path.splitext(os.path.basename(csv_file))[0]
    print(f"Processing {algorithm_name} from {csv_file}")
    
    df = pd.read_csv(csv_file)
    print(f"  Loaded {len(df)} rows from {csv_file}")
    
    # Filter data for the fixed M value
    filtered_data = df[df['M'] == FIXED_M]
    print(f"  After filtering for M={FIXED_M}, {len(filtered_data)} rows remain")
    
    if not filtered_data.empty:
        found_m_value = True
        # Process each test case
        for case in filtered_data['Case'].unique():
            case_data = filtered_data[filtered_data['Case'] == case]
            print(f"  Processing case '{case}' with {len(case_data)} rows")
            
            # Sort by size of second array
            case_data = case_data.sort_values(by='N')
            
            # Collect data points
            for _, row in case_data.iterrows():
                algorithms.append(algorithm_name)
                sizes_n.append(row['N'])
                metrics_values.append(row[metric_column])
                print(f"  Added data point: N={row['N']}, {metric_column}={row[metric_column]}")
    else:
        print(f"No data with M={FIXED_M} found for {algorithm_name}")

# Check if any data with the specified M value was found
if not found_m_value:
    print(f"Error: No data found with M={FIXED_M} in any algorithm.")
    print("Available M values:")
    
    # Show available M values to help the user
    all_m_values = set()
    for csv_file in csv_files:
        df = pd.read_csv(csv_file)
        if 'M' in df.columns:
            all_m_values.update(df['M'].unique())
    
    if all_m_values:
        for m in sorted(all_m_values):
            print(f"  - M={m}")
    else:
        print("  No M values found in CSV files.")
    
    sys.exit(1)

# Create DataFrame for plotting
plot_df = pd.DataFrame({
    'Algorithm': algorithms,
    'SizeN': sizes_n,
    'MetricValue': metrics_values
})

# Set X-axis limits
max_n = plot_df['SizeN'].max() if not plot_df.empty else 10000
min_n = plot_df['SizeN'].min() if not plot_df.empty else 1000

# Round limits to powers of 10 for log scale
x_min = 10 ** np.floor(np.log10(min_n))
x_max = 10 ** np.ceil(np.log10(max_n))

# Calculate Y-axis limits based on data
if not plot_df.empty:
    y_min = plot_df['MetricValue'].min() * 0.9
    y_max = plot_df['MetricValue'].max() * 1.1
    
    # Handle log scale requirements
    if y_min <= 0:
        y_min = plot_df['MetricValue'][plot_df['MetricValue'] > 0].min() * 0.9
        if np.isnan(y_min):
            y_min = 0.001
else:
    # Default limits if no data
    if METRIC == 'comparisons':
        y_min, y_max = 1000, 1000000
    else:
        y_min, y_max = 0.001, 10

# Round Y limits for log scale
if y_min > 0:
    y_min = 10 ** np.floor(np.log10(y_min))
    y_max = 10 ** np.ceil(np.log10(y_max))

# Print summary
print("\nData summary for the plot:")
for algorithm in plot_df['Algorithm'].unique():
    alg_data = plot_df[plot_df['Algorithm'] == algorithm]
    print(f"Algorithm: {algorithm}, Data points: {len(alg_data)}")
    print(f"  N values: {sorted(alg_data['SizeN'].unique())}")
    print(f"  {metric_column} values: {sorted(alg_data['MetricValue'].unique())}")

# Initialize the figure
fig, ax = plt.subplots(figsize=(16, 10), facecolor='white', dpi=100)
ax.set_facecolor('white')

# Set up log scales
ax.set_xscale('log')
ax.set_yscale('log')

# Apply axis limits
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

# Configure log scale ticks
major_x_ticks = [10**i for i in range(int(np.log10(x_min)), int(np.log10(x_max)) + 1)]
major_y_ticks = [10**i for i in range(int(np.log10(y_min)), int(np.log10(y_max)) + 1)]

# Formatter for clean tick labels
def format_ticks(x, pos):
    return f'{int(x)}' if x >= 1 else f'{x:.1f}'

ax.xaxis.set_major_locator(LogLocator(base=10.0, numticks=len(major_x_ticks)))
ax.yaxis.set_major_locator(LogLocator(base=10.0, numticks=len(major_y_ticks)))

# Apply formatters
ax.xaxis.set_major_formatter(FuncFormatter(format_ticks))
ax.yaxis.set_major_formatter(FuncFormatter(format_ticks))

# Style the tick labels
ax.tick_params(axis='both', which='major', labelsize=12)
plt.setp(ax.get_xticklabels(), fontweight='bold')
plt.setp(ax.get_yticklabels(), fontweight='bold')

# Remove unnecessary ticks
ax.tick_params(axis='both', which='minor', length=0)
ax.tick_params(axis='both', which='major', length=0)

# Add grid
ax.grid(True, which='major', color='#CCCCCC', linestyle='-', linewidth=0.8)
ax.grid(False, which='minor')

# Add axis labels
ax.set_xlabel('РАЗМЕР ВТОРОГО МАССИВА', fontsize=14, labelpad=10)

# Set Y-axis label based on metric
if METRIC == 'comparisons':
    y_label = 'КОЛИЧЕСТВО СРАВНЕНИЙ'
else:
    y_label = 'ВРЕМЯ ВЫПОЛНЕНИЯ (мс)'

ax.set_ylabel(y_label, fontsize=14, labelpad=10, rotation=90)

# Plot each algorithm
for algorithm in plot_df['Algorithm'].unique():
    alg_data = plot_df[plot_df['Algorithm'] == algorithm]
    
    # Sort by N for correct line
    alg_data = alg_data.sort_values(by='SizeN')
    
    # Get color for algorithm
    color = COLORS.get(algorithm, 'black')
    
    # Create the plot
    ax.plot(alg_data['SizeN'], alg_data['MetricValue'], 
            label=algorithm, 
            color=color,
            linewidth=2.5,
            marker='o',
            markersize=5)

# Set title
if METRIC == 'comparisons':
    title = f'Среднее количество сравнений, совершаемых\nразными алгоритмами при размере первого\nмассива m = {FIXED_M}'
else:
    title = f'Среднее время выполнения\nразных алгоритмов при размере первого\nмассива m = {FIXED_M}'

ax.set_title(title, fontsize=18, pad=20)

# Add legend
legend = ax.legend(fontsize=14, loc='upper center', bbox_to_anchor=(0.5, -0.15), 
                 frameon=True, facecolor='white', ncol=5)

# Style background
ax.patch.set_facecolor('white')
fig.patch.set_facecolor('white')

# Add border
for spine in ax.spines.values():
    spine.set_visible(True)
    spine.set_color('black')
    spine.set_linewidth(1.0)

# Adjust layout
plt.tight_layout()
plt.subplots_adjust(bottom=0.25, left=0.1, right=0.9)

# Create filename based on metric
if METRIC == 'comparisons':
    filename = f'comparison_fixed_m_{FIXED_M}.png'
else:
    filename = f'time_fixed_m_{FIXED_M}.png'

# Save the plot
plt.savefig(f'{plots_dir}/{filename}', dpi=300, bbox_inches='tight', facecolor='white')
plt.close()

print(f"2D plot of {METRIC} with fixed M={FIXED_M} has been generated with formatting as requested") 