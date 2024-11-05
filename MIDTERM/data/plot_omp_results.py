#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import argparse

def create_individual_timing_plots(df):
    """Create separate timing plots for each matrix size."""
    sizes = df['infile'].unique()
    
    for size in sizes:
        plt.figure(figsize=(10, 6))
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        
        # Plot all three timing measurements
        plt.plot(threads, size_data['time_overall'], marker='o', 
                label='Overall Time', linestyle='-')
        plt.plot(threads, size_data['time_computation'], marker='s',
                label='Computation Time', linestyle='--')
        plt.plot(threads, size_data['time_other'], marker='^',
                label='Other Time', linestyle=':')
        
        plt.grid(True, which="both", ls="-", alpha=0.2)
        plt.xlabel('Number of Threads')
        plt.ylabel('Time (seconds)')
        plt.title(f'OpenMP: Timing Analysis for {size}')
        
        # Turn off scientific notation
        plt.ticklabel_format(style='plain', axis='y')
        plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
        
        plt.legend()
        plt.tight_layout()
        plt.savefig(f'omp_timing_{size}.png', dpi=300)
        plt.close()

def create_speedup_plots(df):
    """Create separate speedup plots for overall and computation times."""
    sizes = df['infile'].unique()
    thread_values = df['threads'].unique()
    
    # Overall speedup plot
    plt.figure(figsize=(10, 6))
    plt.plot(thread_values, thread_values, 'k--', label='Ideal', alpha=0.5)
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_overall = size_data[size_data['threads'] == 1]['time_overall'].iloc[0]
        speedup_overall = t1_overall / size_data['time_overall']
        plt.plot(threads, speedup_overall, marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup')
    plt.title('OpenMP: Overall Speedup vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_speedup_overall.png', dpi=300)
    plt.close()
    
    # Computation speedup plot
    plt.figure(figsize=(10, 6))
    plt.plot(thread_values, thread_values, 'k--', label='Ideal', alpha=0.5)
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_comp = size_data[size_data['threads'] == 1]['time_computation'].iloc[0]
        speedup_comp = t1_comp / size_data['time_computation']
        plt.plot(threads, speedup_comp, marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup')
    plt.title('OpenMP: Computation Speedup vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_speedup_computation.png', dpi=300)
    plt.close()

def create_efficiency_plots(df):
    """Create separate efficiency plots for overall and computation times."""
    sizes = df['infile'].unique()
    thread_values = df['threads'].unique()
    
    # Overall efficiency plot
    plt.figure(figsize=(10, 6))
    plt.axhline(y=1.0, color='k', linestyle='--', label='Ideal', alpha=0.5)
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_overall = size_data[size_data['threads'] == 1]['time_overall'].iloc[0]
        efficiency_overall = (t1_overall / size_data['time_overall']) / threads
        plt.plot(threads, efficiency_overall, marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Efficiency')
    plt.title('OpenMP: Overall Efficiency vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_efficiency_overall.png', dpi=300)
    plt.close()
    
    # Computation efficiency plot
    plt.figure(figsize=(10, 6))
    plt.axhline(y=1.0, color='k', linestyle='--', label='Ideal', alpha=0.5)
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_comp = size_data[size_data['threads'] == 1]['time_computation'].iloc[0]
        efficiency_comp = (t1_comp / size_data['time_computation']) / threads
        plt.plot(threads, efficiency_comp, marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Efficiency')
    plt.title('OpenMP: Computation Efficiency vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_efficiency_computation.png', dpi=300)
    plt.close()

def create_serial_fraction_plots(df):
    """Create separate serial fraction plots for overall and computation times."""
    sizes = df['infile'].unique()
    thread_values = df['threads'].unique()
    
    # Overall serial fraction plot
    plt.figure(figsize=(10, 6))
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_overall = size_data[size_data['threads'] == 1]['time_overall'].iloc[0]
        speedup_overall = t1_overall / size_data['time_overall']
        
        # e = (1/S - 1/p)/(1 - 1/p)
        e_overall = [(1/s - 1/p)/(1 - 1/p) if p > 1 else 0 
                    for s, p in zip(speedup_overall, threads)]
        
        plt.plot(threads[1:], e_overall[1:], marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Serial Fraction (e)')
    plt.title('OpenMP: Overall Serial Fraction vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_serial_fraction_overall.png', dpi=300)
    plt.close()
    
    # Computation serial fraction plot
    plt.figure(figsize=(10, 6))
    
    for size in sizes:
        size_data = df[df['infile'] == size]
        threads = size_data['threads']
        t1_comp = size_data[size_data['threads'] == 1]['time_computation'].iloc[0]
        speedup_comp = t1_comp / size_data['time_computation']
        
        e_comp = [(1/s - 1/p)/(1 - 1/p) if p > 1 else 0 
                 for s, p in zip(speedup_comp, threads)]
        
        plt.plot(threads[1:], e_comp[1:], marker='o', label=f'{size}')
    
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Serial Fraction (e)')
    plt.title('OpenMP: Computation Serial Fraction vs Number of Threads')
    
    # Turn off scientific notation
    plt.ticklabel_format(style='plain')
    plt.gca().yaxis.set_major_formatter(ScalarFormatter(useOffset=False))
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('omp_serial_fraction_computation.png', dpi=300)
    plt.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate OpenMP performance plots.')
    parser.add_argument('input_file', type=str, help='Path to the input CSV file')
    args = parser.parse_args()
    
    # Read the OpenMP data
    try:
        df = pd.read_csv(args.input_file)
    except FileNotFoundError:
        print(f"Error: {args.input_file} not found")
        exit(1)
    
    print("Creating plots...")
    
    create_individual_timing_plots(df)
    create_speedup_plots(df)
    create_efficiency_plots(df)
    create_serial_fraction_plots(df)
    
    print("\nPlots have been created:")
    print("- Individual timing plots for each size (4 files)")
    print("- Overall speedup plot")
    print("- Computation speedup plot")
    print("- Overall efficiency plot")
    print("- Computation efficiency plot")
    print("- Overall serial fraction plot")
    print("- Computation serial fraction plot")