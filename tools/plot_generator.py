import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os
import glob
from scipy.interpolate import griddata, Rbf

# Get all CSV files from results directory
csv_files = glob.glob('results/*.csv')

# Create plots directory if it doesn't exist
plots_dir = 'plots'
if not os.path.exists(plots_dir):
    os.makedirs(plots_dir)

# Process each CSV file
for csv_file in csv_files:
    # Load the CSV data
    df = pd.read_csv(csv_file)
    
    # Get algorithm name from filename
    algorithm_name = os.path.splitext(os.path.basename(csv_file))[0]
    
    # Create figure with 3D projection
    fig = plt.figure(figsize=(14, 10))
    ax = fig.add_subplot(111, projection='3d')
    
    # Process each test case
    for case in df['Case'].unique():
        case_data = df[df['Case'] == case]
        
        # Extract data points
        x = case_data['M'].values
        y = case_data['N'].values
        z = case_data['Comparisons'].values
        
        # Print summary to debug
        print(f"Data summary for {algorithm_name} - {case}:")
        print(f"Number of points: {len(x)}")
        print(f"X range: {min(x)} to {max(x)}")
        print(f"Y range: {min(y)} to {max(y)}")
        print(f"Z range: {min(z)} to {max(z)}")
        
        # Create a tight grid for the surface plot - just enclosing our actual data points
        # Define grid boundaries based on data 
        x_min, x_max = np.min(x), np.max(x)
        y_min, y_max = np.min(y), np.max(y)
        
        # Create logarithmically spaced points only within our data range
        xi = np.logspace(np.log10(x_min), np.log10(x_max), 50)
        yi = np.logspace(np.log10(y_min), np.log10(y_max), 50)
        
        # Create meshgrid
        X, Y = np.meshgrid(xi, yi)
        
        # Use Radial Basis Function interpolation which works better for sparse data
        rbf = Rbf(x, y, z, function='multiquadric')
        Z = rbf(X, Y)
        
        # Create mask for areas too far from data points
        mask = np.ones_like(Z, dtype=bool)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                # Check distance to nearest data point
                min_dist = np.min(np.sqrt((X[i,j] - x)**2 + (Y[i,j] - y)**2))
                if min_dist > 0.5 * max(x_max - x_min, y_max - y_min):
                    mask[i,j] = False
        
        # Apply mask - set masked areas to NaN so they don't display
        Z_masked = np.where(mask, Z, np.nan)
        
        # Plot the 3D surface
        surf = ax.plot_surface(X, Y, Z_masked, 
                           cmap='viridis',
                           rstride=1, 
                           cstride=1,
                           linewidth=0,
                           antialiased=True,
                           alpha=0.9)
        
        # Add scatter points to verify data with larger points
        ax.scatter(x, y, z, color='red', s=50, alpha=0.7)
        
        # Add a color bar
        fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5, label='Количество сравнений')
    
    # Set labels with larger font size
    ax.set_xlabel('M', fontsize=14)
    ax.set_ylabel('N', fontsize=14)
    ax.set_zlabel('Количество сравнений', fontsize=14)
    ax.set_title(f'Количество сравнений для {algorithm_name}', fontsize=16)
    
    # Explicitly set z-axis limits to ensure visibility
    ax.set_zlim(0, max(df['Comparisons']) * 1.1)
    
    # Reverse Y (N) axis direction
    ax.set_ylim(y_max * 1.1, y_min * 0.9)
    
    # Set view angle - N axis facing the viewer
    ax.view_init(elev=20, azim=200)
    
    # Add grid
    ax.grid(True)
    
    # Save only one plot
    plt.savefig(f'{plots_dir}/{algorithm_name}_3d.png', dpi=300, bbox_inches='tight')
    plt.close()

print(f"3D plot has been generated in the '{plots_dir}' directory")
