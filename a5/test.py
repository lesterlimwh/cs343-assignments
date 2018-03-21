from subprocess import check_output
import random
for i in range(1000):
    seed = random.randint(1, 999999)
    print(seed)
    out = check_output(["./vote", "6", "3", "2", str(seed)])
    if b"X" in out:
        print(seed)
        print(out)
