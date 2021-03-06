from Mitarushi_sort_func import *
from random import randint
import matplotlib.pyplot as plt
from tqdm import trange

LENGTH = 10000
FLIP_MAX = 10000
FLIP_MIN = 0
TRY = 1000
sorts = [("Bucket Sort", swap_sort, "blue"),
         ("Quick Sort", quick_sort, "green"),
         ("Comb Sort", comb_sort, "red"),
         ("Shell Sort", shell_sort, "cyan"),
         ("Heap Sort", heap_sort, "magenta"),
         ("Simple Cost", simple_sort_cost, "orange"),
         ("Simple Cost2", simple_sort_cost2, "brown")]
reverse_imfo = ("Inversion Number", get_reverse_count, "yellow")


if __name__ == "__main__":
    result_n = []
    result_sort = [[] for _ in sorts]
    result_reverse = []

    for _ in trange(TRY):
        n = LENGTH
        s = generate_random(n)
        half_sorted(s, randint(FLIP_MIN, FLIP_MAX))

        result_n.append(n)
        result_reverse.append(get_reverse_count(s))

        for i, (_, func, _) in enumerate(sorts):
            result_sort[i].append(func(s[:]))

    plt.figure(figsize=(14, 12))
    ax1 = plt.subplot2grid((2, 7), (0, 0), 2, 3)
    ax2 = plt.subplot2grid((2, 7), (0, 4), 2, 3)

    ax1.set_xlabel("Simple2 Cost")
    ax1.set_ylabel("Sort Cost")
    # ax1.set_xlim(0, 4000000)
    # ax1.set_ylim(0, 16000000)
    ax1.grid()
    for i, (name, _, color) in enumerate(sorts[:-1]):
        ax1.scatter(result_sort[-1], result_sort[i], label=name, c=color, s=1)
    ax1.scatter(result_sort[-1], result_reverse,
                label=reverse_imfo[0], c=reverse_imfo[2], s=1)
    ax1.legend()

    ax2.set_xlabel("Inversion Number")
    ax2.set_ylabel("Sort Cost")
    # ax2.set_xlim(0, 4000000)
    # ax2.set_ylim(0, 8000000)
    ax2.grid()
    # ax2.set_aspect("equal", "datalim")
    for i, (name, _, color) in enumerate(sorts):
        ax2.scatter(result_reverse, result_sort[i], label=name, c=color, s=1)
    ax2.legend()

    plt.savefig("sort_test\\result_const_n.png")
