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
global interrupt16
global interrupt17
global interrupt18
global interrupt19
global interrupt20
global interrupt21
global interrupt22
global interrupt23
global interrupt24
global interrupt25
global interrupt26
global interrupt27
global interrupt28
global interrupt29
global interrupt30
global interrupt31
global interrupt32
global interrupt33


interrupt0:
    cli
    push byte 0
    push byte 0
    jmp interrupt_common

; 1: Debug Exception
interrupt1:
    cli
    push byte 0
    push byte 1
    jmp interrupt_common

; 2: Non Maskable Interrupt Exception
interrupt2:
    cli
    push byte 0
    push byte 2
    jmp interrupt_common

; 3: Int 3 Exception
interrupt3:
    cli
    push byte 0
    push byte 3
    jmp interrupt_common

; 4: INTO Exception
interrupt4:
    cli
    push byte 0
    push byte 4
    jmp interrupt_common

; 5: Out of Bounds Exception
interrupt5:
    cli
    push byte 0
    push byte 5
    jmp interrupt_common

; 6: Invalid Opcode Exception
interrupt6:
    cli
    push byte 0
    push byte 6
    jmp interrupt_common

; 7: Coprocessor Not Available Exception
interrupt7:
    cli
    push byte 0
    push byte 7
    jmp interrupt_common

; 8: Double Fault Exception (With Error Code!)
interrupt8:
    cli
    push byte 8
    jmp interrupt_common

; 9: Coprocessor Segment Overrun Exception
interrupt9:
    cli
    push byte 0
    push byte 9
    jmp interrupt_common

; 10: Bad TSS Exception (With Error Code!)
interrupt10:
    cli
    push byte 10
    jmp interrupt_common

; 11: Segment Not Present Exception (With Error Code!)
interrupt11:
    cli
    push byte 11
    jmp interrupt_common

; 12: Stack Fault Exception (With Error Code!)
interrupt12:
    cli
    push byte 12
    jmp interrupt_common

; 13: General Protection Fault Exception (With Error Code!)
interrupt13:
    cli
    push byte 13
    jmp interrupt_common

; 14: Page Fault Exception (With Error Code!)
interrupt14:
    cli
    push byte 14
    jmp interrupt_common

; 15: Reserved Exception
interrupt15:
    cli
    push byte 0
    push byte 15
    jmp interrupt_common

; 16: Floating Point Exception
interrupt16:
    cli
    push byte 0
    push byte 16
    jmp interrupt_common

; 17: Alignment Check Exception
interrupt17:
    cli
    push byte 0
    push byte 17
    jmp interrupt_common

; 18: Machine Check Exception
interrupt18:
    cli
    push byte 0
    push byte 18
    jmp interrupt_common

; 19: Reserved
interrupt19:
    cli
    push byte 0
    push byte 19
    jmp interrupt_common

; 20: Reserved
interrupt20:
    cli
    push byte 0
    push byte 20
    jmp interrupt_common

; 21: Reserved
interrupt21:
    cli
    push byte 0
    push byte 21
    jmp interrupt_common

; 22: Reserved
interrupt22:
    cli
    push byte 0
    push byte 22
    jmp interrupt_common

; 23: Reserved
interrupt23:
    cli
    push byte 0
    push byte 23
    jmp interrupt_common

; 24: Reserved
interrupt24:
    cli
    push byte 0
    push byte 24
    jmp interrupt_common

; 25: Reserved
interrupt25:
    cli
    push byte 0
    push byte 25
    jmp interrupt_common

; 26: Reserved
interrupt26:
    cli
    push byte 0
    push byte 26
    jmp interrupt_common

; 27: Reserved
interrupt27:
    cli
    push byte 0
    push byte 27
    jmp interrupt_common

; 28: Reserved
interrupt28:
    cli
    push byte 0
    push byte 28
    jmp interrupt_common

; 29: Reserved
interrupt29:
    cli
    push byte 0
    push byte 29
    jmp interrupt_common

; 30: Reserved
interrupt30:
    cli
    push byte 0
    push byte 30
    jmp interrupt_common

; 31: Reserved
interrupt31:
    cli
    push byte 0
    push byte 31
    jmp interrupt_common

	interrupt32:
	cli
	push byte 0
	push byte 32
	jmp interrupt_common

	interrupt33:
	cli
	push byte 0
	push byte 33
	jmp interrupt_common