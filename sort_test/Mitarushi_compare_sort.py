from tqdm import trange
import matplotlib.pyplot as plt
from random import randint
from Mitarushi_sort_func import *
# import sys
# sys.setrecursionlimit(100000000)

MAX_LENGTH = 4000
MIN_LENGTH = 10
TRY = 1000
sorts = [("Bucket Sort", swap_sort, "blue"),
         ("Quick Sort", quick_sort, "green"),
         ("Comb Sort", comb_sort, "red"),
         ("Shell Sort", shell_sort, "cyan"),
         ("Heap Sort", heap_sort, "magenta"), ]
#("Simple Cost", simple_sort_cost, "orange"),
# ("Simple Cost2", simple_sort_cost2, "brown")]
reverse_imfo = ("Inversion Number", get_reverse_count, "yellow")
half_sort = False


if __name__ == "__main__":
    result_n = []
    result_sort = [[] for _ in sorts]
    result_reverse = []

    for _ in trange(TRY):
        n = randint(MIN_LENGTH, MAX_LENGTH)
        s = generate_random(n)
        if half_sort:
            half_sorted(s, max(1, n // 3))

        result_n.append(n)
        result_reverse.append(get_reverse_count(s))

        for i, (_, func, _) in enumerate(sorts):
            result_sort[i].append(func(s[:]))

    plt.figure(figsize=(8, 6))
    ax1 = plt.subplot2grid((1, 1), (0, 0), 1, 1)
    #ax2 = plt.subplot2grid((1, 7), (0, 4), 1, 3)

    ax1.set_xlabel("Length")
    ax1.set_ylabel("Sort Cost")
    ax1.set_xlim(0, MAX_LENGTH)
    ax1.set_ylim(0, 16000000)
    ax1.grid()
    for i, (name, _, color) in enumerate(sorts):
        ax1.scatter(result_n, result_sort[i], label=name, c=color, s=1)
    # ax1.scatter(result_n, result_reverse,
    #             label=reverse_imfo[0], c=reverse_imfo[2], s=1)
    ax1.legend()

    # ax2.set_xlabel("Inversion Number")
    # ax2.set_ylabel("Sort Cost")
    # ax2.set_xlim(0, 4000000)
    # ax2.set_ylim(0, 8000000)
    # ax2.grid()
    # ax2.set_aspect("equal", "datalim")
    # for i, (name, _, color) in enumerate(sorts):
    #     ax2.scatter(result_reverse, result_sort[i], label=name, c=color, s=1)
    # ax2.legend()

    if half_sort:
        plt.savefig("sort_test\\result_all_half.png")
    else:
        plt.savefig("result_all_2.png")
