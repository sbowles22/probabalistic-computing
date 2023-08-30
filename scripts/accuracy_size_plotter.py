import matplotlib.pyplot as plt
import numpy as np
from sys import argv

if __name__ == "__main__":
    with open("acc_1000_400.txt", "r") as f:
        data_raw = f.readlines()

    data = [[float(num) for num in line.split(" ")] for line in data_raw]

    x, y = zip(*data)
    x = [int(i) for i in x]

    """-----------PLOT-----------"""
    fig, ax = plt.subplots()

    ax.set_title(f"Accuracy vs. Graph Size")

    ax.set_xlabel("Size")
    ax.set_xlim(1, 23)
    ax.set_xticks(ticks=[i for i in range(2, 23, 2)], labels=[i for i in range(2, 23, 2)])

    ax.set_ylabel("Accuracy")
    ax.set_ylim(0, 1.0)

    ax.grid()
    ax.scatter(x, y)

    plt.savefig('acc_1000_400.png')
    """---------END PLOT---------"""

