import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from numpy.random import normal
from progressbar import progressbar
from scipy.integrate import BDF, RK45
# import imageio
from sdeint import *
from time import sleep

runs = 2000

"""
Graph configuration
"""
SIZE = 2
MAX_CUT = 1
ORG_GRAPH = np.array([[0, 1], [1, 0]])

# SIZE = 3
# MAX_CUT = 2
#
# ORG_GRAPH = np.array([
#     [0, 1, 1],
#     [1, 0, 1],
#     [1, 1, 0],
# ])

# SIZE = 4
# MAX_CUT = 3
#
# ORG_GRAPH = np.array([
#     [0, 1, 0, 1],
#     [1, 0, 1, 1],
#     [0, 1, 0, 0],
#     [1, 1, 0, 0],
# ])
"""
End of graph configuration
"""

COUPLING_COEFF = 0.1

coupling_coefficients = ORG_GRAPH * -COUPLING_COEFF

starting_amplitudes = np.zeros((SIZE * 2)) * 0
tspan = np.linspace(0, 10, 1001)
noises = np.logspace(1, -5, 61, base=10)


def generate_kramer_moyal(p, zeta):
    def kramer_moyal(y, t):
        c = y[:len(y) // 2]
        s = y[len(y) // 2:]

        dcdt = np.zeros(np.size(c))
        dsdt = np.zeros(np.size(s))

        for i, _ in enumerate(c):
            dcdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * c[i]
            dcdt[i] += sum((zeta[i][j] * c[j] for j, _ in enumerate(c)))

        for i, _ in enumerate(s):
            dsdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * s[i]
            dsdt[i] += sum((zeta[i][j] * s[j] for j, _ in enumerate(s)))

        return np.concatenate((dcdt, dsdt))

    return kramer_moyal


def sample_spherical(npoints, ndim=4):
    vec = np.random.randn(ndim, npoints)
    vec /= np.linalg.norm(vec, axis=0)
    return vec


def gen_quantum_noise(q, size):
    def quantum_noise(y, t):
        # return np.zeros((4,1))
        return np.full((2 * SIZE), q) * sample_spherical(1, size)

    return quantum_noise


def evaluate_energy(answer, graph):
    energy = 0
    for i, partition_i in enumerate(answer):
        for j, partition_j in enumerate(answer):
            # if graph[i][j]:
            #     print(f'Connection between {i} and {j}')
            if i != j and partition_i != partition_j:
                # if graph[i][j]:
                #     print(f'Energy between {i} and {j}')
                energy += graph[i][j]
    return energy // 2


def run():
    for noise in noises:
        print(f'\nTesting noise {noise}...')
        xa = []
        ya = []
        correct = 0
        for _ in progressbar(range(runs)):
            y_probabalistic = stratint(generate_kramer_moyal(1.1, coupling_coefficients), gen_quantum_noise(noise, 2 * SIZE),
                                       starting_amplitudes, tspan)

            xa.append(y_probabalistic[-1][0])
            ya.append(y_probabalistic[-1][1])
            answer = np.sign(y_probabalistic[-1][:SIZE])

            # print('---')
            # print(answer)
            # print(answer * answer[0])
            # print(answer * answer[0] * np.array([1, -1]))
            # print('---')

            # print(answer, evaluate_energy(answer, ORG_GRAPH))

            if evaluate_energy(answer, ORG_GRAPH) == MAX_CUT:
                correct += 1

        yield correct / runs, noise
        # fig, ax = plt.subplots()
        # ax.scatter(xa, ya)
        # ax.set_title(f'{noise=}, Success={correct / runs}, n={runs}')
        # plt.savefig(f'Noise_{noise}.png')


if __name__ == '__main__':
    accuracies = []
    for accuracy, noise in run():
        sleep(0.01)
        print(f'{100 * accuracy:.0f}%, {noise:.1e}')
        accuracies.append(accuracy)

    fig, ax = plt.subplots()
    ax.plot(noises, accuracies)
    ax.invert_xaxis()
    ax.set_xscale('log')
    ax.set_xlabel('Noise')
    ax.set_ylabel('Accuracy')
    ax.set_title(f'Accuracy of Simulated Noisy {SIZE}-OPO Network for MAX-CUT')
    plt.show()

    # fig, ax = plt.subplots()
    # ax.plot(tspan, np.transpose(y_deterministic)[0])
    # ax.plot(tspan, np.transpose(y_probabalistic)[0])
    # ax.plot(tspan, np.transpose(y_xtra_crazy_mode)[0])
    # plt.show()
    #
    # fig, ax = plt.subplots()
    # ax.plot(np.transpose(y_deterministic)[0], np.transpose(y_deterministic)[1])
    # ax.plot(np.transpose(y_probabalistic)[0], np.transpose(y_probabalistic)[1])
    # ax.plot(np.transpose(y_xtra_crazy_mode)[0], np.transpose(y_xtra_crazy_mode)[1])
    # plt.show()
