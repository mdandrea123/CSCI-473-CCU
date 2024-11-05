#!/bin/bash

echo "This will create the same type of results as the midterm report, however the values may be different"

make clean all

echo "Creating 5000x5000 matrix"
./make-2d 5000 5000 5kin.dat
echo "Matrix stored in 5kin.dat"

echo "Creating 10000x10000 matrix"
./make-2d 10000 10000 10kin.dat
echo "Matrix stored in 10kin.dat"

echo "Creating 20000x20000 matrix"
./make-2d 20000 20000 20kin.dat
echo "Matrix stored in 20kin.dat"

echo "Creating 40000x40000 matrix"
./make-2d 40000 40000 40kin.dat
echo "Matrix stored in 40kin.dat"

echo "Gathering pthreads data"
python3 ../data/gather_data_threads.py pth_results.csv
echo "Results stored in pth_results.csv"

echo "Gathering openmp data"
python3 ../data/gather_data_openmp.py omp_results.csv
echo "Results stored in omp_results.csv"

echo "Plotting pthreads data"
python3 ../data/plot_pth_results.py pth_results.csv
echo "Plotting complete"

echo "Plotting openmp data"
python3 ../data/plot_omp_results.py omp_results.csv
echo "Plotting complete"