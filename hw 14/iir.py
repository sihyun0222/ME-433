import matplotlib.pyplot as plt
import numpy as np
import csv

# Smoothing factor
a = 0.9

# Initialize lists for time and data
t = []
data1 = []
data2 = []

# Prompt for filename
filename = input("Enter filename: ")

# Variable for the exponentially averaged value
avg_val = 0

# Read the CSV file
with open(filename + '.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))  # Time column
        data1.append(float(row[1]))  # Raw data column

        # Compute the exponentially weighted average
        avg_val = a * avg_val + (1 - a) * float(row[1])
        data2.append(avg_val)

# Plot the raw and filtered data
plt.plot(t, data1, '-b', label='Raw Data')
plt.plot(t, data2, '-r', label='Filtered Data')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title(f'IIR Filtered Signal for {filename} (α = {a}, 1-α = {1-a})')
plt.legend()
plt.tight_layout()
plt.show()
