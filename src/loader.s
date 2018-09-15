.set MAGIC, 0x1badb002 # 'magic number' lets bootloader find the header
.set FLAGS, (1<<0 | 1<<1) # this is the Multiboot 'flag' field
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern __main__
.extern ctor_init
.global loader


loader:
    mov $kernel_stack, %esp
    call ctor_init
    push %eax
    push %ebx
    call __main__


_stop:
    cli
    hlt
    jmp _stop


.section .bss
.space 2*1024*1024; # 2 MiB
kernel_stack:
