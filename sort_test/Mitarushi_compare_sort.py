from Mitarushi_sort_reverse import *
from random import randint
import matplotlib.pyplot as plt
from tqdm import trange

MAX_LENGTH = 1000
MIN_LENGTH = 10
TRY = 100000

if __name__ == "__main__":
    result_sort = []
    result_reverse = []
    for _ in trange(TRY):
        n = randint(MIN_LENGTH, MAX_LENGTH)
        s = generate_random(n)
        result_sort.append(swap_sort(s[:])[1])
        result_reverse.append(get_reverse_count(s))

    plt.figure(figsize=(12, 6))
    plt.xlabel("Sort Cost")
    plt.ylabel("Inversion Number")
    plt.grid()
    plt.xlim(0, 600000)
    plt.ylim(0, 300000)
    plt.axes().set_aspect("equal", "datalim")
    plt.scatter(result_sort, result_reverse, s=2)
    plt.savefig("sort_test\\result.png")
