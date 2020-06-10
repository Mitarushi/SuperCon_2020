import numpy as np
line_n = 10000
with open("sort_cost/sort_cost.txt") as f:
    result = [list(map(int, i.split(" ")))
              for i in f.read().split("\n")[:-1]]


def get_func(n, simple):
    a = [n ** 2, n, n ** 0.5, 1]
    b = [simple, simple ** 0.5, simple**0.25, 1]
    x = [i * j for j in b for i in a]
    return x


mat_n = 16

A = [[0] * mat_n for _ in range(mat_n)]
b = [0] * mat_n

for n, simple, _, quick in result:
    f = get_func(n, simple)
    for i in range(mat_n):
        for j in range(mat_n):
            A[i][j] += f[i] * f[j]
    for i in range(mat_n):
        b[i] += f[i] * quick

A = np.matrix(A, dtype=np.float64)
b = np.matrix(b, dtype=np.float64)
x = np.linalg.solve(A, b.T)
for i in x:
    print("%.16f," % i.item())

# -0.0000000000005458,
#  0.0000000749720954,
#  -0.0209028079827218,
#  0.0000000455017940,
#  0.2422630682283575,
#  -24.6278565197337578,
#  -0.0128371292462913,
#  -3.3606791939878078,
#  0.2602838553124828
