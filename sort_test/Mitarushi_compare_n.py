from Mitarushi_sort_reverse import *
from random import randint
import matplotlib.pyplot as plt
from tqdm import trange

LENGTH = [4000, 5000, 6000, 7000,
          8000, 9000, 10000]
COLOR = ["brown", "green", "cyan", "blue",
         "yellow", "orange", "red"]
TRY = 1000

func = [simple_sort_cost3, get_reverse_count, quick_sort]


if __name__ == "__main__":
    result_sort = [[[] for _ in LENGTH] for _ in func]

    for ind, n in enumerate(LENGTH):
        for _ in trange(TRY):
            s = generate_random(n)
            half_sorted(s, randint(0, n*3//2))

            for i, f in enumerate(func):
                result_sort[i][ind].append(f(s[:]))

    plt.figure(figsize=(12, 14))
    ax1 = plt.subplot2grid((7, 2), (0, 0), 3, 2)
    ax2 = plt.subplot2grid((7, 2), (4, 0), 3, 2)

    ax1.set_xlabel("Simple Cost2")
    ax1.set_ylabel("Quick Sort Cost")
    # ax1.set_xlim(0, 4000000)
    # ax1.set_ylim(0, 16000000)
    ax1.grid()
    for i, n in enumerate(LENGTH):
        ax1.scatter(result_sort[0][i], result_sort[2][i],
                    label="N = %d" % n, c=COLOR[i], s=1)
    ax1.legend()

    ax2.set_xlabel("Simple Cost2")
    ax2.set_ylabel("Inversion Number")
    # ax2.set_xlim(0, 4000000)
    # ax2.set_ylim(0, 8000000)
    ax2.grid()
    # ax2.set_aspect("equal", "datalim")
    for i, n in enumerate(LENGTH):
        ax2.scatter(result_sort[0][i], result_sort[1][i],
                    label="N = %d" % n, c=COLOR[i], s=1)
    ax2.legend()

    plt.savefig("sort_test\\result_simple3.png")
