#!/usr/bin/env python3

from random import Random

random = Random()

width = 10
height = 10

print("graph G {")

for x in range(0, width):
    for y in range(0, height):
        print(f"{x}.{y};")

for x in range(0, width - 1):
    for y in range(0, height - 1):
        if random.choice([True, True, False]):
            print(f"{x}.{y} -- {x + 1}.{y};")

        if random.choice([True, True, False]):
            print(f"{x}.{y} -- {x}.{y + 1};")

        if random.choice([True, True, False]):
            print(f"{x}.{y} -- {x + 1}.{y + 1};")

for x in range(1, width):
    for y in range(0, height - 1):
        pass
        #print(f"{x}.{y} -- {x - 1}.{y + 1};")


for x in range(0, width - 1):
    print(f"{x}.{height - 1} -- {x + 1}.{height - 1};")

for y in range(0, height - 1):
    print(f"{width - 1}.{y} -- {width - 1}.{y + 1};")

print("}")
