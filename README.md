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
```

#### Notes
- We were not able to jump back into programs after printing, so instead we halt the kernel
