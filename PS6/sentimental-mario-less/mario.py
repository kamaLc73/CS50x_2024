from cs50 import get_int
Height = get_int("Height: ")
while Height not in range(1, 9):
    Height = get_int("Height: ")

for row in range(1, Height + 1):
    for space in range(Height - row):
        print(" ", end="")
    for hash_symbol in range(row):
        print("#", end="")
    print()
