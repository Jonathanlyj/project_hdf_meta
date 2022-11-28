import fputs
fputs.fputs("I'm building Python Extension with C!", "write.txt")

with open("write.txt", "r") as f:
    print(f.read())

