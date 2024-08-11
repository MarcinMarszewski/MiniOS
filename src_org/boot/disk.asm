;load <dh> sectors from drive <dl> into ES:BX

disk_load:
	pusha
	push dx
	mov ah, 0x02 	;'read'
	mov al, dh 		;n of sectors to read
	mov cl, 0x02 	;sector
	mov ch, 0x00	;cylinder
	mov dh, 0x00	;head

	int 0x13		;disk BIOS int
	jc disk_load_error

	pop dx
	cmp al, dh
	jne incorrect_n_of_sectors_error

	popa
	ret

disk_load_error:
	jmp disk_error_loop		;TODO: disk error handling

incorrect_n_of_sectors_error:
	jmp disk_error_loop		;TODO: sector loading error handling

disk_error_loop:
	jmp $					;Should we halt like that
							;probably should reset the cpu
							;or retry loading