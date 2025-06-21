"""
Author: Sergei Gorlov.
Description: Generates 2D plots from CSV results to visualize the number of comparisons and execution time for merge algorithms.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob
import argparse
import sys
from matplotlib.ticker import LogLocator, FuncFormatter, ScalarFormatter

# ====================================================================
# 2D Plot Generator for Merge Algorithm Comparison
# ====================================================================
# This script creates 2D plots comparing merge algorithm performance
# with a double logarithmic scale (log-log).
# Axis ranges are configurable via command line parameters.
# ====================================================================

# Parse command line arguments
parser = argparse.ArgumentParser(description='Generate 2D comparison plot for merge algorithms')
parser.add_argument('--m', type=int, default=1000, help='Fixed size of the first array (default: 1000)')
parser.add_argument('--metric', type=str, choices=['comparisons', 'time'], default='comparisons', 
                    help='Metric to plot: comparisons or execution time (default: comparisons)')
parser.add_argument('--x-min', type=int, help='Minimum value for X-axis (default: auto-detected)')
parser.add_argument('--x-max', type=int, help='Maximum value for X-axis (default: auto-detected)')
parser.add_argument('--y-min', type=float, help='Minimum value for Y-axis (default: auto-detected)')
parser.add_argument('--y-max', type=float, help='Maximum value for Y-axis (default: auto-detected)')
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
    'HwangLinDynamicMerge': '#FF0000',  # Gray
    'HwangLinDynamicStableMerge': '#70AD47',  # Red
    'HwangLinKnuthMerge': '#FFC000',  # Yellow
    'HwangLinStaticMerge': '#70AD47',  # Light Blue
    'HwangLinStaticStableMerge': '#FF00FF',  # Pink
    'SimpleKimKutznerMerge': '#70AD47', # Green
    'StdMerge': '#FF00FF', # Pink
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

# Determine X-axis range - use command line args if provided, otherwise auto-detect
if args.x_min is not None:
    x_min = args.x_min
else:
    x_min = plot_df['SizeN'].min() if not plot_df.empty else 100
    # Ensure x_min is positive for log scale
    if x_min <= 0:
        # Find the smallest positive value
        positive_values = plot_df['SizeN'][plot_df['SizeN'] > 0]
        if not positive_values.empty:
            x_min = positive_values.min()
        else:
            x_min = 1  # Default minimum for log scale

if args.x_max is not None:
    x_max = args.x_max
else:
    x_max = plot_df['SizeN'].max() if not plot_df.empty else 100000
    # Не округлять вверх, а брать максимальное значение из данных

# Calculate Y-axis limits based on data
if not plot_df.empty:
    # Use command line args if provided, otherwise auto-detect
    if args.y_min is not None:
        y_min = args.y_min
    else:
        y_min = plot_df['MetricValue'].min() * 0.9
        # Handle log scale requirements
        if y_min <= 0:
            y_min = plot_df['MetricValue'][plot_df['MetricValue'] > 0].min() * 0.9
            if np.isnan(y_min):
                y_min = 1  # Default if no valid value found
        # Round to nearest power of 10 below
        y_min = 10 ** np.floor(np.log10(y_min))
        
    if args.y_max is not None:
        y_max = args.y_max
    else:
        y_max = plot_df['MetricValue'].max() * 1.1
else:
    # Default limits if no data
    if METRIC == 'comparisons':
        y_min, y_max = 10, 100000
    else:
        y_min, y_max = 0.001, 1000

# Print summary of axis ranges
print(f"\nAxis ranges:")
print(f"  X-axis: {x_min} to {x_max}")
print(f"  Y-axis: {y_min} to {y_max}")

# Print summary of data
print("\nData summary for the plot:")
for algorithm in plot_df['Algorithm'].unique():
    alg_data = plot_df[plot_df['Algorithm'] == algorithm]
    print(f"Algorithm: {algorithm}, Data points: {len(alg_data)}")
    print(f"  N values: {sorted(alg_data['SizeN'].unique())}")
    print(f"  {metric_column} values: {sorted(alg_data['MetricValue'].unique())}")

# Set larger font sizes for all text elements
TITLE_SIZE = 22      # Title font size
AXIS_LABEL_SIZE = 18 # Axis labels font size
TICK_LABEL_SIZE = 14 # Tick labels font size
LEGEND_SIZE = 16     # Legend font size

# Initialize the figure
fig, ax = plt.subplots(figsize=(16, 10), facecolor='white', dpi=100)
ax.set_facecolor('white')

# Set up log scales for both axes - Double logarithmic scale
ax.set_xscale('log')
ax.set_yscale('log')

# Apply axis limits
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

# Configure log scale ticks based on actual range
x_powers = range(int(np.log10(x_min)), int(np.log10(x_max)) + 1)
y_powers = range(int(np.log10(y_min)), int(np.log10(y_max)) + 1)

# Generate major tick positions at powers of 10
x_ticks = [10**i for i in x_powers]
y_ticks = [10**i for i in y_powers]

# Add intermediate log ticks on both axes
x_minor_ticks = []
for major in x_ticks:
    for i in range(2, 10):
        x_minor_ticks.append(major * i)
y_minor_ticks = []
for major in y_ticks:
    for i in range(2, 10):
        y_minor_ticks.append(major * i)

# Set the ticks
x_ticks = sorted(plot_df['SizeN'].unique())
xticks_to_show = [v for v in [25000, 50000, 75000, 100000] if v in x_ticks]
ax.set_xticks(xticks_to_show)

# Вернуть отображение всех уникальных значений по оси Y
# Добавим дополнительные тики 25k, 50k, 75k
extra_yticks = [25000, 50000, 75000]
y_ticks = sorted(set(y_ticks).union(extra_yticks))
ax.set_yticks(y_ticks)

# Formatter for clean tick labels
def format_log_ticks(x, pos):
    if x >= 1000:
        return f'{int(x/1000)}k' if x % 1000 == 0 else f'{x/1000:.1f}k'
    else:
        return f'{int(x)}' if x == int(x) else f'{x:.1f}'

# Apply formatters
ax.xaxis.set_major_formatter(FuncFormatter(format_log_ticks))
ax.yaxis.set_major_formatter(FuncFormatter(format_log_ticks))

# Style the tick labels with larger font size и наклон
ax.tick_params(axis='both', which='major', labelsize=TICK_LABEL_SIZE)
plt.setp(ax.get_xticklabels(), rotation=30, ha='right', fontweight='bold')
plt.setp(ax.get_yticklabels(), fontweight='bold')

# Add grid for log scale - both major and minor
ax.grid(True, which='major', color='#CCCCCC', linestyle='-', linewidth=0.8)
ax.grid(True, which='minor', color='#EEEEEE', linestyle=':', linewidth=0.5)

# Add axis labels with larger font size
ax.set_xlabel('РАЗМЕР ВТОРОГО МАССИВА', fontsize=AXIS_LABEL_SIZE, fontweight='bold', labelpad=15)

# Set Y-axis label based on metric
if METRIC == 'comparisons':
    y_label = 'КОЛИЧЕСТВО СРАВНЕНИЙ'
else:
    y_label = 'ВРЕМЯ ВЫПОЛНЕНИЯ (мс)'

ax.set_ylabel(y_label, fontsize=AXIS_LABEL_SIZE, fontweight='bold', labelpad=15, rotation=90)

# Plot each algorithm with thicker lines for better visibility
algorithms_to_plot = list(plot_df['Algorithm'].unique())
if 'TwoWayMerge' in algorithms_to_plot:
    algorithms_to_plot.remove('TwoWayMerge')
# Сначала рисуем все, кроме TwoWayMerge
for algorithm in algorithms_to_plot:
    alg_data = plot_df[plot_df['Algorithm'] == algorithm]
    alg_data = alg_data.sort_values(by='SizeN')
    color = COLORS.get(algorithm, 'black')
    display_name = algorithm
    if algorithm == 'SimpleKimKutznerMerge':
        display_name = 'KimKutznerMerge'
    elif algorithm == 'FractialInsertionMerge':
        display_name = 'FractialInsertion'
    # Индивидуальные стили для HwangLinStaticMerge и HwangLinStaticStableMerge
    if algorithm == 'HwangLinStaticMerge':
        linestyle = '-'
        linewidth = 5.0
        marker = 'o'
        markersize = 7
        zorder = 5
    elif algorithm == 'HwangLinStaticStableMerge':
        linestyle = (0, (5, 5))  # Штрихпунктирная линия
        linewidth = 5.0
        marker = 's'
        markersize = 8
        zorder = 6
    elif algorithm == 'HwangLinDynamicMerge':
        linestyle = '-'
        linewidth = 5.0
        marker = 'o'
        markersize = 7
        zorder = 5
    elif algorithm == 'HwangLinDynamicStableMerge':
        linestyle = (0, (5, 5))  # Штрихпунктирная линия
        linewidth = 5.0
        marker = 's'
        markersize = 8
        zorder = 6
    else:
        linestyle = '-'
        linewidth = 5.0
        marker = 'o'
        markersize = 6
        zorder = 1
    ax.plot(alg_data['SizeN'], alg_data['MetricValue'],
            label=display_name,
            color=color,
            linewidth=linewidth,
            marker=marker,
            markersize=markersize,
            linestyle=linestyle,
            zorder=zorder)
# Теперь рисуем TwoWayMerge поверх всех
if 'TwoWayMerge' in plot_df['Algorithm'].unique():
    alg_data = plot_df[plot_df['Algorithm'] == 'TwoWayMerge']
    alg_data = alg_data.sort_values(by='SizeN')
    color = COLORS.get('TwoWayMerge', 'black')
    display_name = 'TwoWayMerge'
    linestyle = '-'
    ax.plot(alg_data['SizeN'], alg_data['MetricValue'], 
            label=display_name,
            color=color,
            linewidth=5.0,
            marker='o',
            markersize=6,
            linestyle=linestyle,
            zorder=10)  # Поверх остальных

# Set title with larger font size
if METRIC == 'comparisons':
    title = f'Среднее количество сравнений, совершаемых\nразными алгоритмами при размере первого\nмассива m = {FIXED_M}'
else:
    title = f'Среднее время выполнения\nразных алгоритмами при размере первого\nмассива m = {FIXED_M}'

ax.set_title(title, fontsize=TITLE_SIZE, fontweight='bold', pad=20)

# Create formatter for Y-axis (with scientific notation for large numbers)
if METRIC == 'comparisons' and y_max > 100000:
    def sci_format(x, pos):
        if x >= 1000:
            return f'{x/1000:.0f}k' if x % 1000 == 0 else f'{x/1000:.1f}k'
        else:
            return f'{x:.0f}'
    ax.yaxis.set_major_formatter(FuncFormatter(sci_format))

# Add legend with larger font size and better spacing
legend = ax.legend(fontsize=LEGEND_SIZE, 
                   loc='upper center',
                   bbox_to_anchor=(0.5, -0.18),  # Lower position
                   frameon=True, 
                   facecolor='white', 
                   ncol=3,  # 3 columns but with better spacing
                   handlelength=2.5,  # Longer lines in the legend
                   columnspacing=3.0,  # Much more space between columns
                   handletextpad=0.8,  # Space between the line and the text
                   borderpad=1.0)  # Padding inside the legend frame

# Make legend edges more visible
legend.get_frame().set_linewidth(1.5)
legend.get_frame().set_edgecolor('black')

# Adjust the figure size to be wider for better legend display
fig.set_size_inches(18, 10)  # Wider figure

# Style background
ax.patch.set_facecolor('white')
fig.patch.set_facecolor('white')

# Add border
for spine in ax.spines.values():
    spine.set_visible(True)
    spine.set_color('black')
    spine.set_linewidth(1.5)  # Thicker border

# Adjust layout
plt.tight_layout()
plt.subplots_adjust(bottom=0.30, left=0.1, right=0.9)  # Adjusted bottom margin

# Create filename based on metric
if METRIC == 'comparisons':
    filename = f'comparison_fixed_m_{FIXED_M}.png'
else:
    filename = f'time_fixed_m_{FIXED_M}.png'

# Save the plot
plt.savefig(f'{plots_dir}/{filename}', dpi=300, bbox_inches='tight', facecolor='white')
plt.close()

print(f"2D plot of {METRIC} with fixed M={FIXED_M} has been generated with enlarged axis labels and legend") 