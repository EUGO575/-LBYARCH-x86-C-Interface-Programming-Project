# LBYARCH-x86-C-Interface-Programming-Project
### Demo Video:
- https://drive.google.com/file/d/12NZmxcJ1IfQs13P82aPLEnGUUlGoVIul/view?usp=sharing
Submitted by:
- JACELA, EUGO PASTORPIDE
- PANGAN, VINCE VERGEL ROSALES 

## How to Build and Run
1. **Ensure all files are in the same folder.**

2. **Run the following commands in order:**

   ```bash
   nasm -f win32 imgconv.asm -o imgconv.obj
   gcc -m32 main.c imgconv.obj -o imgconv.exe
   imgconv.exe < input.txt > output.txt

