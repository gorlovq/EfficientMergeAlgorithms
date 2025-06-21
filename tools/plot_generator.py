"""
Author: Sergei Gorlov.
Description: Generates 3D plots from CSV results to visualize the number of comparisons and execution time for merge algorithms.
"""

import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os
import glob
from scipy.interpolate import SmoothBivariateSpline

# Get CSV files and create output directory
csv_files = glob.glob('results/*.csv')

plots_dir = 'plots'
if not os.path.exists(plots_dir):
    os.makedirs(plots_dir)

for csv_file in csv_files:
    df = pd.read_csv(csv_file)
    
    algorithm_name = os.path.splitext(os.path.basename(csv_file))[0]
    
    # Initialize 3D plot
    fig = plt.figure(figsize=(14, 10))
    ax = fig.add_subplot(111, projection='3d')
    
    for case in df['Case'].unique():
        case_data = df[df['Case'] == case]
        
        # Extract data points
        x = case_data['M'].values
        y = case_data['N'].values
        z = case_data['Comparisons'].values
        
        print(f"Data summary for {algorithm_name} - {case}:")
        print(f"Number of points: {len(x)}")
        print(f"X range: {min(x)} to {max(x)}")
        print(f"Y range: {min(y)} to {max(y)}")
        print(f"Z range: {min(z)} to {max(z)}")
        
        # Get data boundaries
        x_min, x_max = np.min(x), np.max(x)
        y_min, y_max = np.min(y), np.max(y)
        
        # Create logarithmic grid for interpolation
        base_x = np.logspace(np.log10(x_min), np.log10(x_max), 50)
        base_y = np.logspace(np.log10(y_min), np.log10(y_max), 50)
        xi = np.sort(np.unique(np.concatenate([base_x, x])))
        yi = np.sort(np.unique(np.concatenate([base_y, y])))
        X, Y = np.meshgrid(xi, yi)

        spline = SmoothBivariateSpline(x, y, z, kx=2, ky=2, s=0)
        Z = spline.ev(X.ravel(), Y.ravel()).reshape(X.shape)
        
        # Create mask to hide areas far from data points
        mask = np.ones_like(Z, dtype=bool)
        max_dist = 0.5 * max(x_max - x_min, y_max - y_min)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                d = np.sqrt((X[i,j] - x)**2 + (Y[i,j] - y)**2)
                if np.min(d) > max_dist:
                    mask[i,j] = False
        Z_masked = np.where(mask, Z, np.nan)
        
        # Plot 3D surface
        surf = ax.plot_surface(
            X, Y, Z_masked,
            cmap='viridis',
            rstride=1,
            cstride=1,
            linewidth=0,
            antialiased=True,
            alpha=0.9
        )
        
        # Add color bar
        cbar = fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5)
        cbar.set_label('Количество сравнений', fontsize=14)
    
    # Set labels and title
    ax.set_xlabel('M', fontsize=14)
    ax.set_ylabel('N', fontsize=14)
    ax.set_zlabel('Количество сравнений', fontsize=14)
    fig.suptitle(f'Количество сравнений для {algorithm_name}', fontsize=16, y=0.88, x=0.57)
    
    # Set axis limits
    ax.set_zlim(0, max(df['Comparisons']) * 1.1)
    ax.set_ylim(y_min * 0.9, y_max * 1.1)
    
    # Set view angle
    ax.view_init(elev=20, azim=200)
    ax.grid(True)

    # Add more space on the left for the z-label
    plt.subplots_adjust(left=0.22)
    
    # Improve layout so title is visually centered
    plt.tight_layout(rect=[0, 0, 1, 0.93])
    
    # Save plot
    plt.savefig(f'{plots_dir}/{algorithm_name}_3d.png', dpi=300, bbox_inches='tight')
    plt.close()

print(f"3D plot has been generated in the '{plots_dir}' directory")