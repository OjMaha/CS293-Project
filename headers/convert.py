
input_file = "../phase1/inputs/mydata.txt"
with open(input_file, 'r') as file:
    for i, line in enumerate(file):
        print(f"{i}: {line.strip()}")
