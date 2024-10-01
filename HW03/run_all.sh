#!/bin/bash

# Compile the program
make clean all

# Define matrix sizes
./make-2d 10 6 small.out
./make-2d 500 100 large.out

# Function to run tests
run_test_small() {
    for i in {1..10} 
    do
        echo "Running test with $i processes..."
        echo "Running test with $i processes..." >> small_results.txt
        mpirun --oversubscribe -np $i ./test-mpi-read small.out >> small_results.txt
    done
}

run_test_large() {
    for i in {1..100} 
    do
        echo "Running test with $i processes..."
        echo "Running test with $i processes..." >> large_results.txt
        mpirun --oversubscribe -np $i ./test-mpi-read large.out >> large_results.txt
    done
}

# Run tests
echo "Running small matrix tests..."
run_test_small

echo "Running large matrix tests..."
run_test_large

echo "All tests completed. Outputs saved to small_results.txt and large_results.txt"
