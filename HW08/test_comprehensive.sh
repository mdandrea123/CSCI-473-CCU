#!/bin/bash

# Make sure executables are built
make clean
make

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Initialize counters
total_tests=0
passed_tests=0

# Create a log file
log_file="test_results.log"
echo "Comprehensive Test Results" > $log_file
date >> $log_file
echo "===================" >> $log_file

# Test matrix dimensions
dimensions=(
    "5 5"   # Square
    "5 10"  # Wide
    "10 5"  # Tall
    "8 8"   # Larger square
    "3 12"  # Very wide
    "12 3"  # Very tall
    "7 7"   # Prime dimensions
    "6 9"   # Composite dimensions
)

# Test different numbers of processes
processes=(1 2 3 4 6 8)

echo "Starting comprehensive tests..."
echo "This may take a while..."

for dim in "${dimensions[@]}"; do
    read rows cols <<< $dim
    echo -e "\n${GREEN}Testing ${rows}x${cols} matrix${NC}"
    echo "Testing ${rows}x${cols} matrix" >> $log_file
    
    # Create input matrix
    ./make-2d $rows $cols A.in
    
    for np in "${processes[@]}"; do
        # Skip if number of processes is greater than rows
        if [ $np -gt $rows ]; then
            continue
        fi
        
        echo -e "  Testing with $np process(es)"
        echo "  Testing with $np process(es)" >> $log_file
        
        # Run the test
        mpirun -np $np ./test-mpi-read A.in A.out A_from_halo.out >> $log_file 2>&1
        
        # Check if output files exist and are identical
        if [ -f A.out ] && [ -f A_from_halo.out ]; then
            if diff A.out A_from_halo.out > /dev/null; then
                echo -e "    ${GREEN}√ Test passed${NC}"
                echo "    Test passed" >> $log_file
                ((passed_tests++))
            else
                echo -e "    ${RED}× Output files differ${NC}"
                echo "    Test failed - Output files differ" >> $log_file
            fi
        else
            echo -e "    ${RED}× Output files not created${NC}"
            echo "    Test failed - Output files not created" >> $log_file
        fi
        
        ((total_tests++))
    done
done

# Print summary
echo -e "\n${GREEN}Testing Complete${NC}"
echo "Total tests run: $total_tests"
echo "Tests passed: $passed_tests"
echo "Success rate: $(( (passed_tests * 100) / total_tests ))%"

# Add summary to log file
echo -e "\nTest Summary" >> $log_file
echo "============" >> $log_file
echo "Total tests run: $total_tests" >> $log_file
echo "Tests passed: $passed_tests" >> $log_file
echo "Success rate: $(( (passed_tests * 100) / total_tests ))%" >> $log_file

echo "Detailed results have been saved to $log_file"