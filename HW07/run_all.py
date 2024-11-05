import subprocess

# Function to run a program with optional arguments
def run_program(command):
    try:
        # Run the command and capture the output
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        # Print the output and return code
        print(f"Running: {' '.join(command)}")
        print(f"Output:\n{result.stdout}")
        print(f"Return Code: {result.returncode}\n")
    except subprocess.CalledProcessError as e:
        # Handle any error that occurs during program execution
        print(f"Error occurred while running: {' '.join(command)}")
        print(f"Stderr: {e.stderr}")

run_program(["make", "clean", "all"])  # Replace arg1, arg2 with actual arguments if any
# Running make-2d
run_program(["./make-2d", "3", "4", "A.dat"])  # Replace arg1, arg2 with actual arguments if any

# Running write-vectror
run_program(["./write-vector", "4", "B.dat"])  # Replace arg1, arg2 with actual arguments

#running omp-matrix-vector
run_program(["./omp-matrix-vector", "4", "A.dat", "B.dat", "C.dat"])  # Replace arg1, arg2 with actual arguments

#running print-vector
run_program(["./print-vector", "C.dat"])  # Replace arg1, arg2 with actual arguments
