import matplotlib.pyplot as plt
import numpy as np
import csv

# Function to read CSV file and extract time and signal data
def read_csv(filename):
    t = []
    s = []
    with open(filename + '.csv') as f:
        reader = csv.reader(f)
        for row in reader:
            t.append(float(row[0])) # leftmost column (time)
            s.append(float(row[1])) # second column (signal)
    return t, s

# Function to compute FFT and frequency range
def compute_fft(t, s):
    Fs = len(t) / t[-1] # Sampling frequency
    Ts = 1.0 / Fs # Sampling interval
    y = np.array(s) # Signal data
    n = len(y) # Length of the signal
    frq = np.fft.fftfreq(n, Ts)[:n//2] # One side frequency range
    Y = np.fft.fft(y) / n # FFT and normalization
    return frq, abs(Y[:n//2])

# Main function to plot signal and FFT
def plot_signal_and_fft(filename):
    t, s = read_csv(filename)
    frq, Y = compute_fft(t, s)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

    # Plotting the signal
    ax1.plot(t, s, 'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Signal vs. Time (' + filename + ')')

    # Plotting the FFT
    ax2.loglog(frq, Y, 'b')
    ax2.set_xlabel('Frequency (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title('FFT (' + filename + ')')

    plt.tight_layout()
    plt.show()

# Prompt for filename and plot
if __name__ == "__main__":
    filename = input("Enter filename: ")
    plot_signal_and_fft(filename)
