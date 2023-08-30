"""
Script to generate heatmaps
Warning, takes ~9 hours to run
"""

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from matplotlib import colors
from numpy.random import normal
from progressbar import progressbar
from scipy.integrate import BDF, RK45
import imageio
from sdeint import *
import imageio

runs = 1000

coupling_coefficients = np.array([[0, -0.1], [-0.1, 0]])

test = np.array([0, 0, 0, 0])
tspan = np.linspace(0, 100, 10001)


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
            dsdt[i] += (-1 - p - (c[i] ** 2 + s[i] ** 2)) * s[i]
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

    # for noise in [1, 0.1, 0.01, 0.001, 0.0001, 0.00001]:
    for noise in [0.1]:
        print('---')
        print(f'Computing Noise {noise}...\n')
        xa = [[] for i in range(100)]
        ya = [[] for i in range(100)]
        # xb = []
        # yb = []
        for j in progressbar(range(runs)):
            # y_deterministic = stratint(generate_kramer_moyal(1.1, coupling_coefficients), gen_quantum_noise(0), test, tspan)
            y_probabalistic = stratint(generate_kramer_moyal(1.1, coupling_coefficients), gen_quantum_noise(noise), test, tspan)
            # y_xtra_crazy_mode = stratint(generate_kramer_moyal(1.1, coupling_coefficients), gen_quantum_noise(0.1), test, tspan)

            # xa.append(y_probabalistic[-1][0])
            # ya.append(y_probabalistic[-1][1])

            for i in range(1, 100):
                xa[i-1].append(y_probabalistic[i*100][0])
                ya[i-1].append(y_probabalistic[i*100][1])


            # xb.append(y_xtra_crazy_mode[-1][0])
            # yb.append(y_xtra_crazy_mode[-1][1])

        # fig, ax = plt.subplots()
        # ax.scatter(xa, ya)
        # # ax.scatter(xb, yb)
        # plt.show()

        print('Saving images...')
        for i in range(1, 100):
            fig, ax = plt.subplots()
            ax.scatter(xa[i-1][:100], ya[i-1][:100])
            ax.set_title(f'Noise={noise:.0E}, t={i/10:.1f}')
            ax.set_xlim(-0.6, 0.6)
            ax.set_ylim(-0.6, 0.6)
            # ax.scatter(xb, yb)
            plt.savefig(f'{i}.png')
            plt.close()

        print('Converting to gif...')
        images = []
        for filename in [f'{i}.png' for i in range(1, 100)]:
            images.append(imageio.v2.imread(filename))
        imageio.mimsave(f'converge_{noise}.gif', images)

        print('Saving heatmap images...')
        for i in range(1, 100):
            fig, ax = plt.subplots()
            ax.hist2d(xa[i - 1], ya[i - 1],
                       bins=100,
                       norm=colors.LogNorm(),
                       cmap="gray")
            ax.set_title(f'Noise={noise:.0E}, t={i / 10:.1f}')
            ax.set_xlim(-0.7, 0.7)
            ax.set_ylim(-0.7, 0.7)
            # ax.scatter(xb, yb)
            plt.savefig(f'{i}.png')
            plt.close()

        print('Converting to heatmap gif...')
        images = []
        for filename in [f'{i}.png' for i in range(1, 100)]:
            images.append(imageio.v2.imread(filename))
        imageio.mimsave(f'heatmap_{noise}.gif', images)

        print()


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
