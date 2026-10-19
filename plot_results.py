import matplotlib.pyplot as plt

# Data from your benchmark run
methods = ['Naive', 'Transpose', 'Tiled', 'Parallel']
times = [1.33508, 0.911925, 0.235298, 0.0560417]
colors = ['#ff9999', '#66b3ff', '#99ff99', '#ffcc99']

# Create the figure
plt.figure(figsize=(10, 6))
bars = plt.bar(methods, times, color=colors, edgecolor='black', alpha=0.7)

# Add title and labels
plt.title('Matrix Multiplication Benchmark (1024x1024)', fontsize=16, fontweight='bold')
plt.ylabel('Execution Time (seconds) - Lower is Better', fontsize=12)
plt.xlabel('Optimization Level', fontsize=12)

# Add annotations on top of bars
base_time = times[0]
for bar, time in zip(bars, times):
    height = bar.get_height()
    speedup = base_time / time
    plt.text(bar.get_x() + bar.get_width()/2., height + 0.02,
             f'{time:.3f}s\n({speedup:.1f}x)',
             ha='center', va='bottom', fontsize=11, fontweight='bold')

# Save the plot
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('benchmark_plot.png')
print("Graph saved as benchmark_plot.png")