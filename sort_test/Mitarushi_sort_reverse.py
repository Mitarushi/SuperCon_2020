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
    print(get_reverse_count(s))
