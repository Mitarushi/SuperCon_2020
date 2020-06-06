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
            cost += abs(j - i)
            # print(j, i)
            s[j], s[i] = s[i], s[j]
            is_sorted[j] = True
    return s, cost


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


def test(s):
    count = 0
    for _ in range(len(s)):
        for i in range(len(s) - 1):
            if s[i] > s[i + 1]:
                s[i], s[i + 1] = s[i + 1], s[i]
                count += 1
    return count


if __name__ == "__main__":
    #s = generate_random(100)
    s = [4, 3, 5, 2, 1]
    print(sorted(s))
    print(swap_sort(s[:]))
    print(get_reverse_count(s))
    print(test(s[:]))
