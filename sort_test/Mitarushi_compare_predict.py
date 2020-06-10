from Mitarushi_sort_func import *
from random import randint
import matplotlib.pyplot as plt
from tqdm import trange

LENGTH = [40000, 50000, 60000, 70000,
          80000, 90000, 100000]
COLOR = ["brown", "green", "cyan", "blue",
         "yellow", "orange", "red"]
TRY = 1000

func = [simple_sort_cost3, get_reverse_count, quick_sort]


if __name__ == "__main__":
    result_sort = [[[] for _ in LENGTH] for _ in func]
    result_correct = [[] for _ in LENGTH]

    for ind, n in enumerate(LENGTH):
        for _ in trange(TRY):
            s = generate_random(n)
            half_sorted(s, randint(0, n*3//2))

            for i, f in enumerate(func):
                result_sort[i][ind].append(f(s[:]))
            result_correct[ind].append(
                simple_cost_correct(n, result_sort[0][ind][-1]))

    plt.figure(figsize=(12, 12))
    ax1 = plt.subplot2grid((1, 1), (0, 0))

    ax1.set_xlabel("Simple Cost2")
    ax1.set_ylabel("Quick Sort Cost")
    # ax1.set_xlim(0, 4000000)
    # ax1.set_ylim(0, 16000000)
    ax1.grid()
    for i, n in enumerate(LENGTH):
        ax1.scatter(result_sort[0][i], result_sort[2][i],
                    label="N = %d" % n, c=COLOR[i], s=1)
        temp = [(i, j) for i, j in zip(result_sort[0][i], result_correct[i])]
        temp.sort()
        ax1.plot([i[0] for i in temp], [i[1] for i in temp], c=COLOR[i])
    ax1.legend()

    plt.savefig("sort_test\\result_correct.png")
