import matplotlib.pyplot as plt
import numpy as np
from sys import argv

if __name__ == "__main__":
    with open("acc_sparsity_200_400.txt", "r") as f:
        data_raw = f.readlines()

    data = [[float(num) for num in line.split(" ")] for line in data_raw]

    x, y = zip(*data)

    """-----------PLOT-----------"""
    fig, ax = plt.subplots()

    ax.set_title(f"Accuracy vs. Graph Mean Sparsity (Size = 16)")

    ax.set_xlabel("Mean Sparsity")
    ax.set_xlim(0, 1.0)

    ax.set_ylabel("Accuracy")
    ax.set_ylim(0, 1.0)

    ax.grid()
    ax.scatter(x, y)

    plt.savefig('acc_spar_200_400.png')
    """---------END PLOT---------"""
