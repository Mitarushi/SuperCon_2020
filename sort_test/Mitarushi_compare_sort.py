from Mitarushi_sort_reverse import *
from random import randint
import matplotlib.pyplot as plt
from tqdm import trange

MAX_LENGTH = 4000
MIN_LENGTH = 10
TRY = 10000
sorts = [("Bucket Sort", swap_sort, "blue"),
         ("Quick Sort", quick_sort, "green"),
         ("Comb Sort", comb_sort, "red"),
         ("Shell Sort", shell_sort, "cyan"),
         ("Heap Sort", heap_sort, "magenta")]
reverse_imfo = ("Inversion Number", get_reverse_count, "yellow")

if __name__ == "__main__":
    result_n = []
    result_sort = [[] for _ in sorts]
    result_reverse = []

    for _ in trange(TRY):
        n = randint(MIN_LENGTH, MAX_LENGTH)
        s = generate_random(n)
        result_n.append(n)
        result_reverse.append(get_reverse_count(s))

        for i, (_, func, _) in enumerate(sorts):
            result_sort[i].append(func(s[:]))

    plt.figure(figsize=(14, 12))
    ax1 = plt.subplot2grid((2, 7), (0, 0), 2, 3)
    ax2 = plt.subplot2grid((2, 7), (0, 4), 2, 3)

    ax1.set_xlabel("Length")
    ax1.set_ylabel("Sort Cost")
    ax1.set_xlim(0, MAX_LENGTH)
    ax1.set_ylim(0, 16000000)
    ax1.grid()
    for i, (name, _, color) in enumerate(sorts):
        ax1.scatter(result_n, result_sort[i], label=name, c=color, s=1)
    ax1.scatter(result_n, result_reverse,
                label=reverse_imfo[0], c=reverse_imfo[2], s=1)
    ax1.legend()

    ax2.set_xlabel("Inversion Number")
    ax2.set_ylabel("Sort Cost")
    ax2.set_xlim(0, 4000000)
    ax2.set_ylim(0, 8000000)
    ax2.grid()
    ax2.set_aspect("equal", "datalim")
    for i, (name, _, color) in enumerate(sorts):
        ax2.scatter(result_reverse, result_sort[i], label=name, c=color, s=1)
    ax2.legend()

    plt.savefig("sort_test\\result_all.png")
