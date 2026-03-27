import sys

for line in sys.stdin:
    try:
        num = int(line.strip())
        print(f"Python received number {num}, squared = {num*num}")
    except ValueError:
        print("Invalid input")