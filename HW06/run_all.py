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
run_program(["./make-2d", "100", "100", "initial100x100x500.dat"])  # Replace arg1, arg2 with actual arguments if any

# Running print-2d
run_program(["./print-2d", "initial100x100x500.dat"])  # Replace arg1, arg2 with actual arguments if any

# Running stencil-2d
run_program(["./stencil-2d", "500", "initial100x100x500.dat", "final100x100x500.dat", "final100x100x500.raw"])  # Replace arg1, arg2 with actual arguments if any

run_program(["python3", "make-movie.py", "final100x100x500.raw", "final100x100x500.mp4"])  # Replace arg1, arg2 with actual arguments if any
