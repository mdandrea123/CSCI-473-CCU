import subprocess
import csv
import re
import argparse

# Function to run a program with optional arguments
def run_program(command):
    try:
        # Run the command and capture the output
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        print(f"Running: {' '.join(command)}")
        print(result.stdout)
        return result.stdout  # Return stdout for parsing
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while running: {' '.join(command)}")
        print(f"Stderr: {e.stderr}")
        return None

# Parse command-line arguments
parser = argparse.ArgumentParser(description="Run pth-stencil-2d and gather data.")
parser.add_argument("output_file", help="The name of the output CSV file")
args = parser.parse_args()

# Define fixed parameters
num_iter = 12
debug_level = 0
infiles = ["../code/5kin.dat", "../code/10kin.dat", "../code/20kin.dat", "../code/40kin.dat"]
thread_counts = [1, 2, 4, 8, 16]

# Prepare CSV file for writing
with open(args.output_file, mode="w", newline="") as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(["infile", "threads", "time_overall", "time_computation", "time_other"])

    # Loop over each infile and thread count and run pth-stencil-2d
    for infile in infiles:
        # Get the base name without "kin.dat" to use in the outfile name
        base_name = infile.replace("kin.dat", "k")
        
        for threads in thread_counts:
            # Generate the outfile name based on infile and thread count
            outfile = f"{base_name}{threads}-threads.dat"
            raw_filename = f"{base_name}{threads}-threads-all.raw"
            command = [
                "../code/pth-stencil-2d",
                str(num_iter),
                infile,
                outfile,
                str(debug_level),
                str(threads),
                raw_filename
            ]
            
            # Run the command and parse the output
            output = run_program(command)
            if output:
                # Use regular expressions to extract times from the output
                overall_time = re.search(r"Time overall = ([\d.e+-]+) seconds", output)
                computation_time = re.search(r"Time computation = ([\d.e+-]+) seconds", output)
                other_time = re.search(r"Time other = ([\d.e+-]+) seconds", output)

                # If times are found, write them to the CSV file
                if overall_time and computation_time and other_time:
                    csv_writer.writerow([
                        infile,
                        threads,
                        overall_time.group(1),
                        computation_time.group(1),
                        other_time.group(1)
                    ])
                    print(f"Recorded times for {infile} with {threads} threads, output file: {outfile}")
                else:
                    print(f"Failed to parse times for {infile} with {threads} threads.")
