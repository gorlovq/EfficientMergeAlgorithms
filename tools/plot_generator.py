import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import os
import glob

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
    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    # Create scatter plot for each test case
    for case in df['Case'].unique():
        case_data = df[df['Case'] == case]
        
        # Plot each individual point
        scatter = ax.scatter(case_data['SizeA'], 
                           case_data['SizeB'], 
                           case_data['Comparisons'],
                           label=f"{case} (n={len(case_data)} tests)",
                           s=100,
                           alpha=1.0)  # Полная непрозрачность
    
    # Set labels
    ax.set_xlabel('M')
    ax.set_ylabel('N')
    ax.set_zlabel('Количество сравнений')
    ax.set_title(f'Количество сравнений для {algorithm_name}')
    
    # Set axis limits
    ax.set_xlim(df['SizeA'].max() * 1.1, 0)  # Разворачиваем ось M
    ax.set_ylim(0, df['SizeB'].max() * 1.1)
    ax.set_zlim(0, df['Comparisons'].max() * 1.1)
    
    # Add legend
    ax.legend()
    
    # Adjust the view
    ax.view_init(elev=15, azim=15)
    
    # Add grid
    ax.grid(True)
    
    # Save plot
    plt.savefig(f'{plots_dir}/{algorithm_name}_3d.png', dpi=300, bbox_inches='tight')
    plt.close()

print(f"3D plots have been generated in the '{plots_dir}' directory")
