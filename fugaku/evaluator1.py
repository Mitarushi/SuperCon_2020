import subprocess
import os

os.chdir("..")
subprocess.run(["g++", "-O2", "fugaku\\main4.cpp"], shell=True)

for i in range(1, 100+1):
    subprocess.run(["a.exe", "<input\\test\\%d.in" % i, ">output\\test\\%d.out" % i],
                   shell=True)
