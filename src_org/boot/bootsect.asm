[bits 16]
[org 0x7c00]
KERNEL_OFFSET equ 0x1000

	mov [BOOT_DRIVE], dl
	mov bp, 0x9000		;setup stack
	mov sp, bp
	

	mov bx, KERNEL_OFFSET	;load kernel
	mov dh, 31				;should we calculate it dynamically?
	mov dl, [BOOT_DRIVE]
	call disk_load

	;call switch to pm
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:init_pm

[bits 32]

init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000	;stack update
	mov esp, ebp
	call BEGIN_PM

BEGIN_PM:
	call KERNEL_OFFSET
	jmp $					;for kernel returning control

BOOT_DRIVE db 0

%include "src/boot/disk.asm"
%include "src/boot/gdt.asm"

times 510 - ($-$$) db 0
dw 0xaa55					;end of bootsector magic word