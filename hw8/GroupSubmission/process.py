# Authors: Dustin Knecht & River Bieberich
# Favorite Location: Trine University Library "The LINK" (1st Floor, Corner by the Stairs)

import sys
import time

CYAN = "\033[36m"
RESET = "\033[0m"

def timestamp():
    return time.strftime("%H:%M:%S")

if len(sys.argv) < 2:
    print("0")
    print(f"{CYAN}[{timestamp()}] Python: Missing argument{RESET}", file=sys.stderr)
    sys.exit(1)

try:
    value = int(sys.argv[1])
except ValueError:
    print("0")
    print(f"{CYAN}[{timestamp()}] Python: Invalid integer{RESET}", file=sys.stderr)
    sys.exit(1)

value += 1
print(value)

print(f"{CYAN}[{timestamp()}] Python processed: {value}{RESET}", file=sys.stderr)