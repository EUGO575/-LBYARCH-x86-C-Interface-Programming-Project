# LBYARCH-x86-C-Interface-Programming-Project
Submission of a project for LBYARCH

## How to Build and Run
1. **Ensure all files are in the same folder.**

2. **Run the following commands in order:**

   ```bash
   nasm -f win32 imgconv.asm -o imgconv.obj
   gcc -m32 main.c imgconv.obj -o imgconv.exe
   imgconv.exe < input.txt > output.txt

