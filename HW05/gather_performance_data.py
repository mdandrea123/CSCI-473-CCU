import subprocess
import time
import sys

def run_simulation(num_processes, num_neutrons):
    cmd = f"mpirun --oversubscribe -np {num_processes} ./nt-parallel -A 0.01 -C 1.0 -H 10.0 -n {num_neutrons}"
    start_time = time.time()
    subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    end_time = time.time()
    return end_time - start_time

def gather_performance_data(output_file):
    process_counts = range(1, 65)
    neutron_counts = [40000000, 80000000, 120000000, 160000000, 200000000]
    start_time = time.time()
    with open(output_file, 'w') as f:
        f.write("processes,neutrons,time\n")
        
        for n in neutron_counts:
            for p in process_counts:
                time_taken = run_simulation(p, n)
                f.write(f"{p},{n},{time_taken}\n")
                print(f"Completed: {p} processes, {n} neutrons, Time: {time_taken:.2f} seconds")

    print(f"Performance data gathering complete. Results saved to {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python gather_performance_data.py <output_file_name>")
        sys.exit(1)
    
    output_file = sys.argv[1]
    gather_performance_data(output_file)