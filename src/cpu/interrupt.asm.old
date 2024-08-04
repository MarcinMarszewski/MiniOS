[extern interrupt_handler]

interrupt_common:
	pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call interrupt_handler
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
	iret
	
global interrupt0
global interrupt1
global interrupt2
global interrupt3
global interrupt4
global interrupt5
global interrupt6
global interrupt7
global interrupt8
global interrupt9
global interrupt10
global interrupt11
global interrupt12
global interrupt13
global interrupt14
global interrupt15


interrupt0:
	cli
	push byte 0
	push byte 0
	jmp interrupt_common

	interrupt1:
	cli
	push byte 0
	push byte 1
	jmp interrupt_common

	interrupt2:
	cli
	push byte 0
	push byte 2
	jmp interrupt_common

	interrupt3:
	cli
	push byte 0
	push byte 3
	jmp interrupt_common

	interrupt4:
	cli
	push byte 0
	push byte 4
	jmp interrupt_common

	interrupt5:
	cli
	push byte 0
	push byte 5
	jmp interrupt_common

	interrupt6:
	cli
	push byte 0
	push byte 6
	jmp interrupt_common

	interrupt7:
	cli
	push byte 0
	push byte 7
	jmp interrupt_common

	interrupt8:
	cli
	push byte 0
	push byte 8
	jmp interrupt_common

	interrupt9:
	cli
	push byte 0
	push byte 9
	jmp interrupt_common

	interrupt10:
	cli
	push byte 0
	push byte 10
	jmp interrupt_common

	interrupt11:
	cli
	push byte 0
	push byte 11
	jmp interrupt_common

	interrupt12:
	cli
	push byte 0
	push byte 12
	jmp interrupt_common

	interrupt13:
	cli
	push byte 0
	push byte 13
	jmp interrupt_common

	interrupt14:
	cli
	push byte 0
	push byte 14
	jmp interrupt_common

	interrupt15:
	cli
	push byte 0
	push byte 15
	jmp interrupt_common