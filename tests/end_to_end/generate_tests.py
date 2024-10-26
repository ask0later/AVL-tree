import random

for num_test in range(1, 11):
    keys = []
    requests1 = []
    requests2 = []

    num_keys = 300
    max_key = 100

    num_requests = num_keys * 3

    for i in range(num_keys):
        keys.append(random.randint(0, max_key))

    for i in range(num_requests):
        requests1.append(random.randint(0, max_key - 2))
        requests2.append(random.randint(requests1[-1], max_key))

    file = open(str(num_test) + ".dat", "w")
    for i in range(num_keys):
        file.write("k " + str(keys[i]) + "\n")
        for j in range(3):
            file.write("q " + str(requests1[i + j]) + " " + str(requests2[i + j]) + "\n")
    file.write("\n")
    file.close()