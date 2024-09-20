import csv
import sys
import matplotlib.pyplot as plt
import numpy as np

def parse_results(filename):
    results = {}
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            n = int(row[0])
            p = int(row[1])
            time = float(row[5])
            if n not in results:
                results[n] = []
            results[n].append((p, time))
    return results

def plot_timing(results, max_n, max_p):
    plt.figure(figsize=(10, 6))
    
    for n, values in results.items():
        values.sort()  # Sort by number of processes P
        p_values = [val[0] for val in values]
        times = [val[1] for val in values]
        plt.plot(p_values, times, marker='o', label=f'N = {n}')
    
    # Plot the ideal curve (assuming linear scaling)
    ideal_p = np.array(list(range(1, max_p + 1)))
    ideal_times = times[0] / ideal_p  # ideal speedup is perfect linear scaling
    plt.plot(ideal_p, ideal_times, 'k--', label="Ideal", linewidth=2)

    plt.xlabel('Number of Processes (P)')
    plt.ylabel('Time (seconds)')
    plt.title('Timing Plot')
    plt.legend()
    plt.grid(True)

    plt.savefig(f'timing_{max_n}_{max_p}.png')
    print(f'Timing plot saved as timing_{max_n}_{max_p}.png')
    plt.close()

def plot_speedup(results, max_n, max_p):
    plt.figure(figsize=(10, 6))

    for n, values in results.items():
        values.sort()  # Sort by number of processes P
        p_values = [val[0] for val in values]
        times = [val[1] for val in values]
        speedup = [times[0] / t for t in times]
        plt.plot(p_values, speedup, marker='o', label=f'N = {n}')
    
    ideal_p = np.array(list(range(1, max_p + 1)))
    plt.plot(ideal_p, ideal_p, 'k--', label="Ideal", linewidth=2)  # Ideal speedup is linear
    
    plt.xlabel('Number of Processes (P)')
    plt.ylabel('Speedup')
    plt.title('Speedup Plot')
    plt.legend()
    plt.grid(True)

    plt.savefig(f'speedup_{max_n}_{max_p}.png')
    print(f'Speedup plot saved as speedup_{max_n}_{max_p}.png')
    plt.close()

def plot_efficiency(results, max_n, max_p):
    plt.figure(figsize=(10, 6))

    for n, values in results.items():
        values.sort()  # Sort by number of processes P
        p_values = [val[0] for val in values]
        times = [val[1] for val in values]
        speedup = [times[0] / t for t in times]
        efficiency = [s / p for s, p in zip(speedup, p_values)]
        plt.plot(p_values, efficiency, marker='o', label=f'N = {n}')
    
    ideal_p = np.array(list(range(1, max_p + 1)))
    ideal_efficiency = np.ones(len(ideal_p))  # Ideal efficiency is 1
    plt.plot(ideal_p, ideal_efficiency, 'k--', label="Ideal", linewidth=2)

    plt.xlabel('Number of Processes (P)')
    plt.ylabel('Efficiency')
    plt.title('Efficiency Plot')
    plt.legend()
    plt.grid(True)

    plt.savefig(f'efficiency_{max_n}_{max_p}.png')
    print(f'Efficiency plot saved as efficiency_{max_n}_{max_p}.png')
    plt.close()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python ./plot_results.py <results txt file from the bash script>")
        sys.exit(1)

    results_file = sys.argv[1]
    results = parse_results(results_file)

    # Get the maximum N and P values for naming the output files
    max_n = max(results.keys())
    max_p = max([max(val[0] for val in values) for values in results.values()])

    plot_timing(results, max_n, max_p)
    plot_speedup(results, max_n, max_p)
    plot_efficiency(results, max_n, max_p)
