# Copyright (C) 2023 Advanced Micro Devices, Inc
#
# SPDX-License-Identifier: MIT


import numpy as np
import csv
import matplotlib.pyplot as plt

class FileGen:
    """
    - Generate the input data file for AIE EMU or read the output data from AIE EMU.

    Attributes:
    - samples (int): Number of samples, default is 1024.
    """
    def __init__(self, samples=1024):
        """
        Initializes the FileGen object with the specified number of samples.

        Parameters:
        - samples (int): Number of samples, default is 1024.
        """
        self.samples = samples

    def read_cmpx(self, file: str, samples: int):
        """
        Reads complex data from the specified file and returns an array.

        Parameters:
        - file (str): The file path.
        - samples (int): Number of samples to read.

        Returns:
        - np.ndarray: Array of complex data.
        """
        value = np.zeros(samples, dtype=np.complex128)
        count = 0
        with open(file, 'r') as f:
            reader = csv.reader(f, delimiter=" ")
            for line in reader:
                if 'T' in line[0]:
                    continue
                for i in range(0, len(line), 2):
                    if line[i] != '':
                        value[count] = float(line[i]) + 1j * float(line[i + 1])
                        count = count + 1
        return value

    def read_int16(self, file: str, samples: int):
        """
        Reads int16 data from the specified file and returns an array.

        Parameters:
        - file (str): The file path.
        - samples (int): Number of samples to read.

        Returns:
        - np.ndarray: Array of int16 data.
        """
        value = np.zeros(samples, dtype=np.int16)
        count = 0
        with open(file, 'r') as f:
            reader = csv.reader(f, delimiter=" ")
            for line in reader:
                if 'T' in line[0]:
                    continue
                for i in range(0, len(line)):
                    if line[i] != '':
                        value[count] = np.int16(line[i])
                        count += 1
        return value

    def write_refoutput(data: np.array, file: str, plio=32, bits=16, scale=True):
        """
        Write complex data to a file.

        Parameters:
        - data (np.array): Complex data samples.
        - file (str): Output filename.
        - plio (int): Bit width of the PLIO port.
        - bits (int): Bit precision per sample.
        - scale (bool): Whether to scale the signal to use full precision.

        Returns:
        - int: Value used for scaling (vscale).
        """
        max_value = np.max(np.abs(data))
        vscale = 1
        if scale:
            vscale = 2 ** int(np.floor(np.log2((1 << (bits - 1)) / max_value)))
            data *= vscale

        with open(file, 'w', newline='') as f:
            for i, v in enumerate(data):
                r, c = np.int16(v.real), np.int16(v.imag)
                f.write("{} {} ".format(r, c))
                # if (((i+1) % 4) == 0 and plio == 128) or (((i+1) % 2) == 1 and plio == 64):
                f.write('\n')

        return vscale

    def vector2file_cint16(data: np.array, file: str, plio=32, bits=16, scale=True):
        """
        Write complex int16 data to a file.

        Parameters:
        - data (np.array): Complex data samples.
        - file (str): Output filename.
        - plio (int): Bit width of the PLIO port.
        - bits (int): Bit precision per sample.
        - scale (bool): Whether to scale the signal to use full precision.

        Returns:
        - int: Value used for scaling (vscale).
        """
        max_value = np.max(np.abs(data))
        vscale = 2 ** int(np.floor(np.log2((1 << (bits - 1)) / max_value)))
        data = data * (vscale if scale else 1)

        with open(file, 'w', newline='') as f:
            for i, v in enumerate(data):
                r, c = np.int16(v.real), np.int16(v.imag)
                f.write("{} {} ".format(r, c))
                # if (((i+1) % 4) == 0 and plio == 128) or (((i+1) % 2) == 1 and plio == 64):
                f.write('\n')

        return vscale

    def vector2file_int16(data: np.array, file: str, plio=32, bits=16, scale=True):
        """
        Write int16 data to a file.

        Parameters:
        - data (np.array): Int16 data samples.
        - file (str): Output filename.
        - plio (int): Bit width of the PLIO port.
        - bits (int): Bit precision per sample.
        - scale (bool): Whether to scale the signal to use full precision.

        Returns:
        - int: Value used for scaling (vscale).
        """
        max_value = np.max(np.abs(data.real))
        vscale = 2 ** int(np.floor(np.log2((1 << (bits - 1)) / max_value)))
        data = data * (vscale if scale else 1)

        with open(file, 'w', newline='') as f:
            for i, v in enumerate(data):
                r = np.int16(v.real)
                f.write("{} ".format(r))
                if (((i + 1) % 8 == 0 and plio == 128) or
                        ((i + 1) % 2 == 0 and (plio == 64 or plio == 32))):
                    f.write('\n')

        return vscale

    def array2header(data: np.array, data_type, header_file_path):
        """
        Generate a header file for an array of data.

        Parameters:
        - data (np.array): Data samples.
        - data_type (str): Data type for the array.
        - header_file_path (str): Output header file path.
        """
        with open(header_file_path, 'w') as file:
            file.write('#ifndef SIGNAL_DATA_H\n')
            file.write('#define SIGNAL_DATA_H\n\n')
            file.write(f'static const {data_type} signal_data[] = {{\n')
            for value in data:
                file.write(f'    {value},\n')
            file.write('};\n\n')
            file.write(f'static const int num_samples = {len(data)};\n')
            file.write('#endif // SIGNAL_DATA_H\n')



