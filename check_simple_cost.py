def simple_cost(s):
    s = list(s)
    cost = 0
    for ind, i in enumerate(s):
        cost += (26 - (ord(i) - ord("a"))) * ind
    s.sort()
    for ind, i in enumerate(s):
        cost -= (26 - (ord(i) - ord("a"))) * ind
    return cost

while True:
    s = input("s: ")
    print(simple_cost(s))