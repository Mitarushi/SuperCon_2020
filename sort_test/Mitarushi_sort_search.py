def dfs(s, cost_max, cost, step):
    length = len(s)
    for i in range(length - 1):
        if s[i] > s[i + 1]:
            break
    else:
        print(cost, len(step), step)
        return

    for i in range(length):
        for j in range(i + 1, length):
            if cost_max < cost + abs(j - i):
                continue
            s[i], s[j] = s[j], s[i]
            step.append((i, j))
            dfs(s, cost_max, cost + abs(j - i), step)
            s[i], s[j] = s[j], s[i]
            step.pop()


if __name__ == "__main__":
    dfs([4, 3, 5, 2, 1], 6, 0, [])
