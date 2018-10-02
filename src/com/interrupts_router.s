.set IRQ_BASE, 0x20
.section .text
.extern _ZN16InterruptManager6handleEhj

.global _ZN16InterruptManager22ignoreInterruptRequestEv

.macro handleException num
.global _ZN16InterruptManager16handleException\num\()Ev
_ZN16InterruptManager16handleException\num\()Ev:
    movb $\num, (interruptId)
    jmp int_bottom
.endm 

.macro handleInterruptRequest num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
_ZN16InterruptManager26handleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptId)
    jmp int_bottom
.endm

handleInterruptRequest 0x00
handleInterruptRequest 0x01
handleInterruptRequest 0x0C

int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptId)
    call _ZN16InterruptManager6handleEhj
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager22ignoreInterruptRequestEv:

    iret

.data
    interruptId: .byte 0
