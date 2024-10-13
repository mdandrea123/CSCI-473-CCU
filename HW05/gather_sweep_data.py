import subprocess
import numpy as np
import sys

def run_simulation(A, C, H, n):
    cmd = f"./nt-serial -A {A} -C {C} -H {H} -n {n}"
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    lines = result.stdout.strip().split('\n')
    return [float(line.split(':')[1].strip('%')) for line in lines]

def gather_data(output_file):
    C = 10.0
    n = 1000
    A_range = np.arange(0.01, 0.51, 0.01)
    H_range = np.arange(0.01, 10.01, 0.01)

    total_iterations = len(A_range) * len(H_range)
    completed_iterations = 0

    with open(output_file, 'w') as f:
        f.write("A,H,Reflected,Absorbed,Transmitted\n")
        for A in A_range:
            for H in H_range:
                reflected, absorbed, transmitted = run_simulation(A, C, H, n)
                f.write(f"{A},{H},{reflected},{absorbed},{transmitted}\n")
                
                completed_iterations += 1
                progress = (completed_iterations / total_iterations) * 100
                print(f"\rProgress: {progress:.2f}% complete", end="", flush=True)

    print("\nData gathering complete. Results saved to {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python gather_sweep_data.py <output_file_name>")
        sys.exit(1)
    
    output_file = sys.argv[1]
    gather_data(output_file)
    print(f"Results saved to {output_file}")