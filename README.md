## 275 Project 2

#### Notes
- We were not able to get `eret` working to jump into programs after printing, exiting, or alarm interrupts. For this submission we are just halting after these.
- We directly used `allocate` and `deallocate` methods instead of `malloc` and `free`

### First test: setup, syscalls, executing programs (no alarm interrupts)
Demonstrates functionality of ram division (free block) linked list, as well as run() and print() methods, and heap initialization. We use `allocate` and `deallocate` to create the linked lists. 

#### Build and run instructions
```
f-assemble bios.asm
f-build combo.vmx kernel-stub.asm heapalloc.c kernel.c
f-build hello-rom.vmx hello.c syscall-wrappers.asm
f-build bao-rom.vmx bao.c syscall-wrappers.asm 
f-build abo-rom.vmx abo.c syscall-wrappers.asm

f-simulate bios.vmx combo.vmx hello-rom.vmx bao-rom.vmx abo-rom.vmx
```
#### Expected output
```
Fivish BIOS v.2024-01-17
(c) Scott F. Kaplan / sfkaplan@amherst.edu
  Searching device table for kernel...done.
  Copying kernel into RAM...done.
  Vectoring into kernel.
Fivish kernel v.2025-03-02
COSC-275 : Systems-II
Initializing trap table...done.
Searching for ROM #00000003
Running program...
Searching for ROM #00000004
Running program...
Searching for ROM #00000005
Running program...
hi
Halting kernel..._
```

### Second test: alarm interrupts
We currently have alarm interrupt functionality commented out. To turn it back on, bring back the commented out portion in kernel.c lines 206-208. It should then reach an alarm interrupt and halt. 
