#!/bin/bash

make clean all

# Define the matrix sizes and number of processes to test
matrix_sizes=("7 6" "10 5" "6 4" "5 7" "10 8")
num_processes=(2 3 4 5)

# Loop over each matrix size
for size in "${matrix_sizes[@]}"; do
  # Extract number of rows and columns
  IFS=' ' read -r rows cols <<< "$size"
  
  # Generate the input matrix
  input_file="input_${rows}x${cols}.in"
  ./make-2d $rows $cols $input_file
  
  # Loop over each number of processes
  for np in "${num_processes[@]}"; do
    echo "Testing with matrix size ${rows}x${cols} and $np processes"
    output_file="output_${rows}x${cols}_p${np}.out"
    halo_output_file="halo_output_${rows}x${cols}_p${np}.out"
    mpirun --oversubscribe -np $np ./test-mpi-read $input_file $output_file $halo_output_file
  done
done

mkdir -p tests
mv *.in *.out tests/

echo "All tests completed successfully. Output files are in the tests/ directory."