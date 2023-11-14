# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT
#
# Description: This class is to generate the signals with noise and plot the signals.

import numpy as np
import matplotlib.pyplot as plt

class SignalGenerator:

    """
    A class for generating complex and real signals.

    Attributes:
    - samples (int): Number of samples in the generated signals.
    - fs (int): Sampling frequency of the signals.
    - signal (np.array): The generated signal.

    Methods:
    - __init__(self, samples=2048, fs=44100): Initializes the SignalGenerator object with default or specified parameters.
    - generate_cmpx(samples=1024, fs=100): Generates a complex signal composed of multiple tones.
    - generate_real(samples=1024, fs=100): Generates a real signal composed of multiple tones.

    Usage Example:
    signal_gen = SignalGenerator(samples=2048, fs=44100)
    complex_signal, time_complex = signal_gen.generate_cmpx()
    real_signal, time_real = signal_gen.generate_real()
    """

    def __init__(self, samples=2048, fs=44100):
        self.samples = samples
        self.fs = fs
        self.signal = self.generate_signal()

    def generate_cmpx(samples = 1024, fs = 100):
        f0, f1, f2, f3, f4 = 0.5, 2.5, 15.3, 16.7, 23.45
        ts = 1 / fs
        time = np.linspace(0, samples*ts, samples)

        tone_0 = 1 * np.cos(2*np.pi*f0*time)
        tone_1 = 0.2 * np.sin(2*np.pi*f1*time+0.1)
        tone_2 = 0.2 * np.sin(2*np.pi*f2*time)
        tone_3 = 0.1 * np.sin(2*np.pi*f3*time+0.1)
        tone_4 = 0.1 * np.sin(2*np.pi*f4*time+0.8)
        signal = (tone_0 + tone_1 + tone_2*1 + tone_3*1j + tone_4*1j)/2
        return signal,time
    
    def generate_real(samples = 1024, fs = 100):
        f0, f1, f2, f3, f4 = 0.5, 2.5, 15.3, 16.7, 23.45
        ts = 1 / fs
        time = np.linspace(0, samples*ts, samples)

        tone_0 = 1 * np.cos(2*np.pi*f0*time)
        tone_1 = 0.2 * np.sin(2*np.pi*f1*time+0.1)
        tone_2 = 0.2 * np.sin(2*np.pi*f2*time)
        tone_3 = 0.1 * np.sin(2*np.pi*f3*time+0.1)
        tone_4 = 0.1 * np.sin(2*np.pi*f4*time+0.8)
        signal = (tone_0 + tone_1 + tone_2*1 + tone_3*1 + tone_4*1)/2
        return signal,time 

class SignalPlotter:

    """
    A class for plotting signals.

    Attributes:
    - signal1 (np.array): First signal to be plotted.
    - signal2 (np.array): Second signal to be plotted.

    Methods:
    - __init__(self, signal1=None, signal2=None): Initializes the SignalPlotter object with specified signals.
    - plot_real_imaginary(num_samples=128): Plots the real and imaginary parts of a signal.
    - plot_signal(num_samples=128): Plots a signal.
    - compare_signal(num_samples=128): Compares two signals.

    Usage Example:
    plotter = SignalPlotter(signal1=complex_signal, signal2=filtered_signal)
    plotter.plot_real_imaginary()
    """
       
    def __init__(self, signal1 = None, signal2 = None):
        self.signal1 = signal1
        self.signal2 = signal2
    
    def plot_real_imaginary(self, num_samples=128):
        plt.figure(figsize=(10, 5))
        plt.plot(self.signal1.real[:num_samples], label='Real')
        plt.plot(self.signal1.imag[:num_samples], label='Imaginary')
        plt.xlabel('Sample')
        plt.ylabel('Amplitude')
        plt.title('Real and Imaginary Parts of the Signal')
        plt.legend()
        plt.grid(True)
        plt.show()

    def plot_signal(self, num_samples=128):
        plt.figure(figsize=(10, 5))
        plt.plot(self.signal1.real[:num_samples], label='Real')
        plt.xlabel('Sample')
        plt.ylabel('Amplitude')
        plt.title('Signal')
        plt.legend()
        plt.grid(True)
        plt.show()

    def compare_signal(self, num_samples=128):
        plt.figure(figsize=(10, 5))
        plt.plot(self.signal1[:num_samples], label='Original Signal')
        plt.plot(self.signal2[:num_samples], label='filtered Signal')
        plt.xlabel('Sample')
        plt.ylabel('Amplitude')
        plt.title('Compare Signal')
        plt.legend()
        plt.grid(True)
        plt.show()
