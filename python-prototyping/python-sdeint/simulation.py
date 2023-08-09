import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from numpy.random import normal
from progressbar import progressbar
from scipy.integrate import BDF, RK45
# import imageio
from sdeint import *

runs = 150

coupling_coefficients = np.array([[0, -0.1], [-0.1, 0]])

test = np.array([0, 0, 0, 0])
tspan = np.linspace(0, 10, 10001)


def generate_kramer_moyal(p, zeta):
    def kramer_moyal(y, t):
        c = y[:len(y) // 2]
        s = y[len(y) // 2:]
        dcdt = np.zeros(np.size(c))
        dsdt = np.zeros(np.size(s))
        # print(y)
        # print(c)
        # print(s)

        for i, _ in enumerate(c):
            dcdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * c[i]
            dcdt[i] += sum((zeta[i][j] * c[j] for j, _ in enumerate(c)))
            # dcdt[i] += normal(0, 1)

        for i, _ in enumerate(s):
            dsdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * s[i]
            dsdt[i] += sum((zeta[i][j] * s[j] for j, _ in enumerate(s)))
            # dsdt[i] += normal(0, 1)

        return np.concatenate((dcdt, dsdt))

    return kramer_moyal


def sample_spherical(npoints, ndim=4):
    vec = np.random.randn(ndim, npoints)
    vec /= np.linalg.norm(vec, axis=0)
    return vec

def gen_quantum_noise(q):
    def quantum_noise(y, t):
        # return np.zeros((4,1))
        return np.array([[q], [q], [q], [q]]) * sample_spherical(1)
    return quantum_noise


if __name__ == '__main__':

    for noise in [0.2, 0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001, 0.00001]:
        print(f'\nTesting noise {noise}...')
        xa = []
        ya = []
        xb = []
        yb = []
        correct = 0
        for _ in progressbar(range(runs)):
            y_probabalistic = stratint(generate_kramer_moyal(1.1, coupling_coefficients), gen_quantum_noise(noise), test, tspan)

            xa.append(y_probabalistic[-1][0])
            ya.append(y_probabalistic[-1][1])
            answer = np.sign(y_probabalistic[-1][:2])

            # print('---')
            # print(answer)
            # print(answer * answer[0])
            # print(answer * answer[0] * np.array([1, -1]))
            # print('---')

            if np.all(answer * answer[0] * np.array([1, -1]) > 0):
                correct += 1

        fig, ax = plt.subplots()
        ax.scatter(xa, ya)
        ax.set_title(f'{noise=}, Success={correct/runs}, n={runs}')
        plt.savefig(f'Noise_{noise}.png')

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
