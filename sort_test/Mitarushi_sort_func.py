from random import randint


def generate_random(length):
    return [randint(0, 25) for _ in range(length)]


def swap_sort(s):
    count_char = [0] * 26
    for i in s:
        count_char[i] += 1

    char_from = [0]
    for i in count_char:
        char_from.append(char_from[-1] + i)

    is_sorted = [False] * len(s)
    cost = 0
    i = 0
    while i < len(s):
        if is_sorted[i]:
            i += 1
        else:
            j = char_from[s[i]]
            char_from[s[i]] += 1
            if s[i] != s[j]:
                cost += abs(j - i)
                s[j], s[i] = s[i], s[j]
            is_sorted[j] = True
    return cost


def med3(x, y, z):
    return sorted([x, y, z])[1]


def quick_sort(s, l=0, r=-1):
    if r == -1:
        r = len(s) - 1

    cost = 0
    if l < r:
        i, j = l, r
        pivot = s[randint(l, r)]
        while True:
            while s[i] < pivot:
                i += 1
            while s[j] > pivot:
                j -= 1
            if i >= j:
                break
            if s[i] != s[j]:
                cost += abs(j - i)
                s[i], s[j] = s[j], s[i]
            i += 1
            j -= 1
        cost += quick_sort(s, l, i - 1)
        cost += quick_sort(s, j + 1, r)
    return cost


def comb_sort(s):
    h = len(s) * 10 // 13
    cost = 0
    while True:
        is_swaped = False
        for i in range(len(s) - h):
            if s[i] > s[i + h]:
                s[i], s[i + h] = s[i + h], s[i]
                cost += h
                is_swaped = True
        if h == 1 and (not is_swaped):
            return cost
        h = max(1, h * 10 // 13)


def shell_sort(s):
    n = len(s)
    h = 1
    while h * 3 + 1 < n:
        h = h * 3 + 1

    cost = 0
    while True:
        for i in range(h):
            for j in range(i, n, h):
                while j >= h and s[j - h] > s[j]:
                    s[j - h], s[j] = s[j], s[j - h]
                    j -= h
                    cost += h
        if h == 1:
            return cost
        else:
            h = (h - 1) // 3


def heap_sort(s):
    n = len(s)
    cost = 0
    for i in range(n):
        while i > 0:
            j = (i - 1) // 2
            if s[j] < s[i]:
                s[i], s[j] = s[j], s[i]
                cost += abs(j - i)
            else:
                break
            i = j

    for i in reversed(range(n)):
        if s[i] != s[0]:
            s[0], s[i] = s[i], s[0]
            cost += i

        j = 0
        while True:
            l = j * 2 + 1
            r = j * 2 + 2
            if l >= i:
                break

            tmp = j
            if r < i and s[r] > s[j]:
                tmp = r
            if s[l] > s[j] and s[l] > s[r]:
                tmp = l

            if tmp == j:
                break
            cost += abs(tmp - j)
            s[tmp], s[j] = s[j], s[tmp]
            j = tmp
    return cost


def half_sorted(s, k):
    h = len(s) * 10 // 13
    while True:
        for i in range(len(s) - h):
            if s[i] > s[i + h]:
                s[i], s[i + h] = s[i + h], s[i]
                k -= 1
                if k <= 0:
                    return
        h = max(1, h * 10 // 13)
        if h == 1:
            return


def simple_sort_cost(s):
    count_char = [0] * 26
    for i in s:
        count_char[i] += 1

    char_from = [0]
    for i in count_char:
        char_from.append(char_from[-1] + i)

    cost = 0
    for ind, i in enumerate(s):
        cost += abs(ind - char_from[i])
    return cost


def simple_sort_cost2(s):
    cost = 0
    for ind, i in enumerate(s):
        cost += (26 - i) * ind
    return cost // 26


# sはランダムだと仮定するとうまくいく 25.5-iを文字の分布によって変えると正確になるかも
def simple_sort_cost3(s):
    cost = 0
    for ind, i in enumerate(s):
        cost += (26 - i) * ind
    s.sort()
    for ind, i in enumerate(s):
        cost -= (26 - i) * ind
    return cost


def simple_cost_correct(n, simple):
    a = [n ** 2, n, n ** 0.5, 1]
    b = [simple, simple ** 0.5, simple**0.25, 1]
    x = [i * j for j in b for i in a]
    c = [-0.0000000000008402,
         0.0000004798000799,
         -0.0002000268185879,
         0.0044613262787533,
         -0.0000001652505149,
         0.2437505930922388,
         -7.7352385687962633,
         25.8180687932298483,
         0.0000356935469576,
         -5.5227029004491737,
         1456.5673308701468613,
         -47616.8398519225956989,
         -0.0145529321527654,
         224.8752720847048465,
         -54970.5081915919436142,
         2140886.3656231933273375]
    correct = 0
    for i, j in zip(x, c):
        correct += i * j
    return correct


class BIT:
    def __init__(self, a, func=int.__add__, one=0):
        self.n = len(a)
        self.func = func
        self.one = one
        self.a = [0] * (self.n + 1)

        for i, j in enumerate(a):
            self.update(i, j)

    def update(self, i, x):
        i += 1
        while i <= self.n:
            self.a[i] = self.func(self.a[i], x)
            i += i & (-i)

    def get(self, i):
        i += 1
        s = self.one
        while i != 0:
            s = self.func(s, self.a[i])
            i -= i & (-i)
        return s


def get_reverse_count(s):
    bit = BIT([0] * 26)
    count = 0
    for ind, i in enumerate(s):
        count += ind-bit.get(i)
        bit.update(i, 1)
    return count


if __name__ == "__main__":
    s = generate_random(100)
    #s = [4, 3, 5, 2, 1]
    print(sorted(s))
    print("bucket", swap_sort(s[:]))
    print("quick", quick_sort(s[:]))
    print("comb", comb_sort(s[:]))
    print("shell", shell_sort(s[:]))
    print("heap", heap_sort(s[:]))
    print("simple cost2", simple_sort_cost2(s[:]))
    print(get_reverse_count(s))
