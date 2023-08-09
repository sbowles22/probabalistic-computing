#! /usr/bin/python3

import matplotlib.pyplot as plt
from sys import argv

if __name__ == "__main__":
    if (len(argv) != 2):
        raise OSError("Include filepath")
    
    with open(argv[1], "r") as f:
        data_raw = f.readlines()

    data = [[float(num) for num in line.split(" ")] for line in data_raw]

    x, y = zip(*data)

    plt.scatter(x, y)
    plt.savefig('PLT.png')
