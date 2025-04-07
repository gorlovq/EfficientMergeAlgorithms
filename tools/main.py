import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV data.
df = pd.read_csv('build/TwoWayMerge_results.csv')

# Print a preview.
print(df.head())

plt.figure(figsize=(10, 6))
plt.plot(df['SizeA'] + df['SizeB'], df['Comparisons'], marker='o', linestyle='-')
plt.xlabel('Size (B)')  # Or change to the appropriate size column.
plt.ylabel('Average Comparisons')
plt.title('Average Comparisons vs. Size')
plt.grid(True)
plt.show()
