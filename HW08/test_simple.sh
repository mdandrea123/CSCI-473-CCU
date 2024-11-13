#!/bin/bash

# Make sure executables are built
make clean all

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Running simple tests..."

# Test 1: 5x6 matrix
echo -e "\n${GREEN}Test 1: 5x6 matrix${NC}"
./make-2d 5 6 A.in
echo "Original matrix:"
./print-2d A.in

# Test with different numbers of processes
for np in 1 2 3; do
    echo -e "\n${GREEN}Testing with $np process(es)${NC}"
    mpirun -np $np ./test-mpi-read A.in A.out A_from_halo.out
    
    echo -e "\nRegular output file:"
    ./print-2d A.out
    echo -e "\nHalo version output file:"
    ./print-2d A_from_halo.out
    
    # Verify files are identical
    if diff A.out A_from_halo.out > /dev/null; then
        echo -e "${GREEN}√ Output files match${NC}"
    else
        echo -e "${RED}× Output files differ${NC}"
    fi
done

# Test 2: 4x4 matrix (square matrix test)
echo -e "\n${GREEN}Test 2: 4x4 matrix${NC}"
./make-2d 4 4 A.in
echo "Original matrix:"
./print-2d A.in

# Test with different numbers of processes
for np in 1 2 4; do
    echo -e "\n${GREEN}Testing with $np process(es)${NC}"
    mpirun -np $np ./test-mpi-read A.in A.out A_from_halo.out
    
    echo -e "\nRegular output file:"
    ./print-2d A.out
    echo -e "\nHalo version output file:"
    ./print-2d A_from_halo.out
    
    # Verify files are identical
    if diff A.out A_from_halo.out > /dev/null; then
        echo -e "${GREEN}√ Output files match${NC}"
    else
        echo -e "${RED}× Output files differ${NC}"
    fi
done

# Test 3: 3x8 matrix (wide matrix test)
echo -e "\n${GREEN}Test 3: 3x8 matrix${NC}"
./make-2d 3 8 A.in
echo "Original matrix:"
./print-2d A.in

# Test with different numbers of processes
for np in 1 2 3; do
    echo -e "\n${GREEN}Testing with $np process(es)${NC}"
    mpirun -np $np ./test-mpi-read A.in A.out A_from_halo.out
    
    echo -e "\nRegular output file:"
    ./print-2d A.out
    echo -e "\nHalo version output file:"
    ./print-2d A_from_halo.out
    
    # Verify files are identical
    if diff A.out A_from_halo.out > /dev/null; then
        echo -e "${GREEN}√ Output files match${NC}"
    else
        echo -e "${RED}× Output files differ${NC}"
    fi
done

echo -e "\n${GREEN}Simple tests completed${NC}"