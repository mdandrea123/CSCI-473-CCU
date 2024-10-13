import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def set_axis_style(ax, x_label, y_label, title):
    ax.set_xlabel(x_label, fontsize=12)
    ax.set_ylabel(y_label, fontsize=12)
    ax.set_title(title, fontsize=14)
    ax.tick_params(axis='both', which='major', labelsize=10)
    ax.grid(True, which="major", ls="-", alpha=0.2)
    ax.legend(fontsize=10, loc='upper left')

def plot_timing(df):
    fig, ax = plt.subplots(figsize=(10, 8))
    colors = ['blue', 'orange', 'green', 'red', 'purple']
    
    neutron_counts = sorted(df['neutrons'].unique())
    for i, n in enumerate(neutron_counts):
        subset = df[df['neutrons'] == n]
        ax.plot(subset['processes'], subset['time'], marker='.', linestyle='-', color=colors[i], label=f'{n//1000000}M')
    
    ax.set_xlim(0, 64)
    ax.set_xticks(range(0, 65, 10))
    
    set_axis_style(ax, 'Process Count (p)', 'Time (seconds)', 'Execution Time vs Process Count')
    
    plt.tight_layout()
    plt.savefig('timing_plot.png')
    plt.close()

def plot_speedup(df):
    fig, ax = plt.subplots(figsize=(10, 8))
    colors = ['blue', 'orange', 'green', 'red', 'purple']
    
    neutron_counts = sorted(df['neutrons'].unique())
    for i, n in enumerate(neutron_counts):
        subset = df[df['neutrons'] == n]
        base_time = subset[subset['processes'] == 1]['time'].values[0]
        speedup = base_time / subset['time']
        ax.plot(subset['processes'], speedup, marker='.', linestyle='-', color=colors[i], label=f'{n//1000000}M')
    
    # Add ideal speedup line
    ax.plot([0, 64], [0, 64], 'k--', label='Ideal')
    
    ax.set_xlim(0, 64)
    ax.set_ylim(0, 64)
    ax.set_xticks(range(0, 65, 10))
    ax.set_yticks(range(0, 65, 10))
    
    set_axis_style(ax, 'Process Count (p)', 'Speedup', 'Speedup vs Process Count')
    
    plt.tight_layout()
    plt.savefig('speedup_plot.png')
    plt.close()

def plot_efficiency(df):
    fig, ax = plt.subplots(figsize=(10, 8))
    colors = ['blue', 'orange', 'green', 'red', 'purple']
    
    neutron_counts = sorted(df['neutrons'].unique())
    for i, n in enumerate(neutron_counts):
        subset = df[df['neutrons'] == n]
        base_time = subset[subset['processes'] == 1]['time'].values[0]
        efficiency = base_time / (subset['time'] * subset['processes'])
        ax.plot(subset['processes'], efficiency, marker='.', linestyle='-', color=colors[i], label=f'{n//1000000}M')
    
    # Add ideal efficiency line
    ax.plot([0, 64], [1, 1], 'k--', label='Ideal')
    
    ax.set_xlim(0, 64)
    ax.set_ylim(0, 1.1)
    ax.set_xticks(range(0, 65, 10))
    ax.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1.0])
    
    set_axis_style(ax, 'Process Count (p)', 'Efficiency', 'Efficiency vs Process Count')
    
    plt.tight_layout()
    plt.savefig('efficiency_plot.png')
    plt.close()

def main(input_file):
    df = pd.read_csv(input_file)
    plot_timing(df)
    plot_speedup(df)
    plot_efficiency(df)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plot_performance_data.py <input_file_name>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    main(input_file)
    print("Performance plotting complete. Check the current directory for output PNG files.")