import subprocess

p = subprocess.Popen(["./cpp_middle"], stdin=subprocess.PIPE, text=True)

p.stdin.write("Hello from Python!\n")
p.stdin.write("This is a 3‑language pipe chain.\n")
p.stdin.close()
p.wait()