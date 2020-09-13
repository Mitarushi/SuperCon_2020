from random import randint
from Mitarushi_sort_func import quick_sort, simple_sort_cost

n = 10**6
s = [randint(0, 25) for _ in range(n)]

print(simple_sort_cost(s))
print(quick_sort(s))
