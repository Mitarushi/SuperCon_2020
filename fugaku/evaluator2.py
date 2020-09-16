import os
os.chdir("..\\output\\test")

result = []
for fn in os.listdir():
    with open(fn) as f:
        res = f.read().split()
        if res[0] == "YES":
            result.append(int(res[-4]))
print(result)
print(sum(result) / len(result))
