#!/bin/bash

# Compile the program
make clean all

# Run the serial parameter sweep and store it in a csv file
echo "Running serial parameter sweep"
python gather_sweep_data.py sweep_data.csv

# Plot the serial parameter sweep data
echo "Plotting serial parameter sweep data"
python plot_sweep_data.py sweep_data.csv

# Run the performance sweep and store the data in a csv file
echo "Running performance sweep"
python gather_performance_data.py performance_data.csv

# Plot the performance sweep data
echo "Plotting performance sweep data"
python plot_performance_data.py performance_data.csv

echo "Script complete."