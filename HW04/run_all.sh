#!/bin/bash

# Compile the program
make clean all

# Define matrix sizes
./make-2d 10 10 A.in
./make-2d 100 100 B.in

# Function to run tests
run_test_small() {
    for i in {1..10} 
    do
        echo "Running test with $i processes..."
        mpirun --oversubscribe -np $i ./test-mpi-read A.in A.out >> A_results.txt
        diff A.in A.out
        if [ $? -eq 0 ]; then
            echo "Success: Files match!"
        else
            echo "Error: Files do not match!"
        fi
    done
}

run_test_large() {
    for i in {1..100} 
    do
        echo "Running test with $i processes..."
        mpirun --oversubscribe -np $i ./test-mpi-read B.in B.out >> B_results.txt
        diff B.in B.out
        if [ $? -eq 0 ]; then
            echo "Success: Files match!"
        else
            echo "Error: Files do not match!"
        fi
    done
}

# Run tests
echo "Running small matrix tests (10x10)..."
run_test_small

echo " "
echo " "

echo "Running large matrix tests (100x100)..."
run_test_large

echo "All tests complete! Results are in A_results.txt and B_results.txt for less clutter."