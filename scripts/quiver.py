import matplotlib.pyplot as plt
from sys import argv

if __name__ == "__main__":
    with open("u.txt", "r") as f:
        data_u_raw = f.readlines()
    with open("v.txt", "r") as f:
        data_v_raw = f.readlines()

    data_u = [[float(num) for num in line.strip().split(" ")] for line in data_u_raw]
    data_v = [[float(num) for num in line.strip().split(" ")] for line in data_v_raw]

    plt.quiver(data_u, data_v)
    plt.savefig("QUIVER.png")
