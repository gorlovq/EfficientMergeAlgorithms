#!/usr/bin/env python3
"""
Author: Sergei Gorlov (edits by Igor Stikentzin).
Description: Generates 2D plots from CSV results to visualize the number of comparisons and execution time for merge algorithms.
"""

import argparse, glob, os, sys
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FuncFormatter

# ─────────────────────────────────────────────────────────────
# Args
# ─────────────────────────────────────────────────────────────
parser = argparse.ArgumentParser(
    description='Generate 2D comparison plot for merge algorithms'
)
parser.add_argument('--m', type=int, default=1000,
                    help='Fixed size of the first array (default: 1000)')
parser.add_argument('--metric', choices=['comparisons', 'time'],
                    default='comparisons',
                    help='Metric to plot (default: comparisons)')
parser.add_argument('--x-min', type=int)
parser.add_argument('--x-max', type=int)
parser.add_argument('--y-min', type=float)
parser.add_argument('--y-max', type=float)
args = parser.parse_args()

# ─────────────────────────────────────────────────────────────
# Data
# ─────────────────────────────────────────────────────────────
csv_files = glob.glob('results/*.csv')
if not csv_files:
    sys.exit("No CSV files found in 'results/'")

plots_dir = 'plots'
os.makedirs(plots_dir, exist_ok=True)

FIXED_M   = args.m
METRIC    = args.metric
COL_NAME  = 'Comparisons' if METRIC == 'comparisons' else 'Time(ms)'

COLORS = {
    'TwoWayMerge': '#4472C4',
    'FractialInsertionMerge': '#ED7D31',
    'HwangLinDynamicMerge': '#A5A5A5',
    'HwangLinDynamicStableMerge': '#FF0000',
    'HwangLinKnuthMerge': '#FFC000',
    'HwangLinStaticMerge': '#5B9BD5',
    'HwangLinStaticStableMerge': '#FF00FF',
    'SimpleKimKutznerMerge': '#70AD47',
}

records = []
for path in csv_files:
    alg = os.path.splitext(os.path.basename(path))[0]
    df = pd.read_csv(path)
    df = df[df['M'] == FIXED_M]
    if df.empty:
        continue
    for case in df['Case'].unique():
        for _, row in df[df['Case'] == case].sort_values('N').iterrows():
            records.append({'Algorithm': alg,
                            'SizeN': row['N'],
                            'MetricValue': row[COL_NAME]})
plot_df = pd.DataFrame(records)
if plot_df.empty:
    sys.exit(f'No data with M={FIXED_M}')

# ─────────────────────────────────────────────────────────────
# Axis ranges
# ─────────────────────────────────────────────────────────────
x_min = args.x_min or plot_df['SizeN'].min()
x_max = args.x_max or plot_df['SizeN'].max()
x_min = 10 ** np.floor(np.log10(x_min))
x_max = 10 ** np.ceil(np.log10(x_max))

y_min = args.y_min if args.y_min is not None else plot_df['MetricValue'].min()*0.9
if y_min <= 0:
    y_min = plot_df['MetricValue'][plot_df['MetricValue']>0].min()*0.9
y_max = args.y_max if args.y_max is not None else plot_df['MetricValue'].max()*1.05

# ─────────────────────────────────────────────────────────────
# Fonts
# ─────────────────────────────────────────────────────────────
TITLE_SIZE      = 28
AXIS_LABEL_SIZE = 24
TICK_LABEL_SIZE = 22
LEGEND_SIZE     = 26

# ─────────────────────────────────────────────────────────────
# Figure
# ─────────────────────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(20, 11), dpi=100, facecolor='white')
ax.set_facecolor('white')
ax.set_xscale('log'); ax.set_yscale('log')
ax.set_xlim(x_min, x_max); ax.set_ylim(y_min, y_max)

# Tick format
def fmt_log(x, _p): return f'{x/1000:g}k' if x >= 1000 else f'{x:g}'
ax.xaxis.set_major_formatter(FuncFormatter(fmt_log))
ax.yaxis.set_major_formatter(FuncFormatter(fmt_log))

# 1) major-ticks
ax.tick_params(axis='both', which='major',
               labelsize=TICK_LABEL_SIZE, pad=8)
# 2) minor-ticks
ax.tick_params(axis='both', which='minor',
               labelsize=TICK_LABEL_SIZE, pad=5)

for lbl in (ax.get_xticklabels(minor=False) + ax.get_xticklabels(minor=True) +
            ax.get_yticklabels(minor=False) + ax.get_yticklabels(minor=True)):
    lbl.set_fontsize(TICK_LABEL_SIZE)
    lbl.set_fontweight('bold')

ax.grid(True, which='major', color='#CCC', linewidth=0.8)
ax.grid(True, which='minor', color='#EEE', linestyle=':', linewidth=0.5)

# Axis signs
ax.set_xlabel('РАЗМЕР ВТОРОГО МАССИВА',
              fontsize=AXIS_LABEL_SIZE, fontweight='bold')
ax.set_ylabel('КОЛИЧЕСТВО СРАВНЕНИЙ' if METRIC == 'comparisons'
              else 'ВРЕМЯ ВЫПОЛНЕНИЯ (мс)',
              fontsize=AXIS_LABEL_SIZE, fontweight='bold')

# Graph lines
for alg in plot_df['Algorithm'].unique():
    sub = plot_df[plot_df['Algorithm'] == alg].sort_values('SizeN')
    name = {'SimpleKimKutznerMerge':'KimKutznerMerge',
            'FractialInsertionMerge':'FractialInsertion'}.get(alg, alg)
    ax.plot(sub['SizeN'], sub['MetricValue'],
            label=name,
            color=COLORS.get(alg, 'black'),
            linewidth=3,
            marker='o', markersize=8)

# Title
title = (f'Среднее количество сравнений при m = {FIXED_M}'
         if METRIC == 'comparisons'
         else f'Среднее время выполнения при m = {FIXED_M}')
ax.set_title(title, fontsize=TITLE_SIZE, fontweight='bold', pad=16)

# Legend
lgd = ax.legend(fontsize=LEGEND_SIZE,
                loc='upper center', bbox_to_anchor=(0.5, -0.24),
                ncol=3, frameon=True, facecolor='white',
                handlelength=3.5, columnspacing=4,
                handletextpad=1.2, borderpad=1.4)
lgd.get_frame().set_linewidth(1.5); lgd.get_frame().set_edgecolor('black')

for spine in ax.spines.values():
    spine.set_visible(True)
    spine.set_linewidth(1.5)
    spine.set_color('black')

# ─────────────────────────────────────────────────────────────
# Save
# ─────────────────────────────────────────────────────────────
plt.tight_layout()
plt.subplots_adjust(top=0.92, bottom=0.30, left=0.1, right=0.95)

fname = (f'comparison_fixed_m_{FIXED_M}.png'
         if METRIC == 'comparisons'
         else f'time_fixed_m_{FIXED_M}.png')
plt.savefig(os.path.join(plots_dir, fname), dpi=300, bbox_inches='tight')
plt.close()
print(f'Plot saved to {plots_dir}/{fname}')
