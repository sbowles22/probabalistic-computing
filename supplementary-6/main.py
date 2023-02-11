import matplotlib.pyplot as plt
import numpy as np
from numpy.random import normal
from progressbar import progressbar
from scipy.integrate import BDF


coupling_coefficients = np.array([[0, -0.1], [-0.1, 0]])

test = np.zeros(4)


def generate_kramer_moyal(p, zeta):
    def kramer_moyal(t, y):
        c = y[:len(y) // 2]
        s = y[len(y) // 2:]
        dcdt = np.zeros(np.size(c))
        dsdt = np.zeros(np.size(s))

        for i, _ in enumerate(c):
            dcdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * c[i]
            dcdt[i] += sum((zeta[i][j] * c[j] for j, _ in enumerate(c)))
            dcdt[i] += normal(0, 1)

        for i, _ in enumerate(s):
            dsdt[i] += (-1 + p - (c[i] ** 2 + s[i] ** 2)) * s[i]
            dsdt[i] += sum((zeta[i][j] * s[j] for j, _ in enumerate(s)))
            dsdt[i] += normal(0, 1)

        return np.concatenate((dcdt, dsdt))

    return kramer_moyal


if __name__ == '__main__':

    in_phase_opo_1 = []
    in_phase_opo_2 = []
    t_in_phases_opos = []

    for _ in progressbar(range(1000)):
        out = BDF(generate_kramer_moyal(1.1, coupling_coefficients), 0, test, 1, max_step=0.01)

        try:
            while out.status != 'finished':
                out.step()
                # in_phase_opo_1.append(out.y[0])
                # in_phase_opo_2.append(out.y[1])
                # t_in_phases_opos.append(out.t)

            in_phase_opo_1.append(out.y[0])
            in_phase_opo_2.append(out.y[1])
        except:
            pass

    # print(out.t)
    # print(out.y)
    fig, ax = plt.subplots()
    ax.hist2d(in_phase_opo_1, in_phase_opo_2)

    # plt.scatter(in_phase_opo_1, in_phase_opo_2)  #, c=t_in_phases_opos)
    plt.show()

    # for i in range(100):
    #     # get solution step state
    #     solution.step()
    #     t_values.append(solution.t)
    #     y_values.append(solution.y[0])
    #     # break loop after modeling is finished
    #     if solution.status == 'finished':
    #         break