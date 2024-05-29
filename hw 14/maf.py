import matplotlib.pyplot as plt
import numpy as np
import csv

# Number of data points for the moving average filter
n = 20

# Initialize lists for time and data
t = []
data1 = []
data2 = []
filter = [0] * n

# Prompt for filename
filename = input("Enter filename: ")

# Counter for tracking the current position in the filter
count = 0

# Read the CSV file
with open(filename + '.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))  # Time column
        data1.append(float(row[1]))  # Raw data column

        # Update the moving average filter
        filter[count % n] = float(row[1])
        val = sum(filter) / n
        data2.append(val)
        count += 1

# Plot the raw and filtered data
plt.plot(t, data1, '-b', label='Raw Data')
plt.plot(t, data2, '-r', label='Filtered Data')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title(f'Moving Average Filter for {filename} (Averaging over {n} Points)')
plt.legend()
plt.tight_layout()
plt.show()