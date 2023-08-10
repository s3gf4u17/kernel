.set MAGIC, 0x1badb002 # sign program as kernel
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
    mov $kernel_stack , %esp # set stack pointer
    call callConstructors
    push %eax
    push %ebx
    call kernelMain # jump to kernelMain function

_stop: # infinite loop to not exit the kernel
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024; # 2 MiB
kernel_stack:
