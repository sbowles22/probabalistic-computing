import matplotlib.pyplot as plt
import numpy as np
from sys import argv

if __name__ == "__main__":
    with open("u.txt", "r") as f:
        data_u_raw = f.readlines()
    with open("v.txt", "r") as f:
        data_v_raw = f.readlines()

    data_u = [[float(num) for num in line.strip().split(" ")] for line in data_u_raw]
    data_v = [[float(num) for num in line.strip().split(" ")] for line in data_v_raw]

    data_u = np.array(data_u)
    data_v = np.array(data_v)

    sf = np.sqrt(data_u**2 + data_v**2)
    sf += 0.000001

    data_u /= sf
    data_v /= sf

    plt.quiver(data_u, data_v)
    plt.savefig("QUIVER.png")
