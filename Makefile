C_SOURCES = $(wildcard src/*.c src/drivers/*.c src/cpu/*.c src/lib/*.c)
HEADERS = $(wildcard src/*.h src/drivers/*.h src/cpu/*.h src/lib/*.h)
OBJ = ${C_SOURCES:.c=.o src/cpu/interrupt.o}


CFLAGS = 
LINKER = i686-elf-ld
GCC = i686-elf-gcc
VM = qemu-system-i386

fresh: clean run

run: mydisk.raw
	${VM} -hda mydisk.raw  -audiodev dsound,id=snd0 -machine pcspk-audiodev=snd0


mydisk.raw: os-image.bin
	qemu-img create -f raw mydisk.raw 128M
	dd if=os-image.bin of=mydisk.raw bs=512 conv=notrunc

os-image.bin: src/boot/bootsect.bin src/kernel.bin
	cat $^ > os-image.bin

src/kernel.bin: src/boot/kernel_entry.o ${OBJ}
	${LINKER} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o src/boot/disk.o ${OBJ}
	${LINKER} -o $@ -Ttext 0x1000 $^ 


%.o: %.c ${HEADERS}
	${GCC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf src/boot/*.bin src/boot/*.o src/cpu/*.o src/drivers/*.o src/lib/*.o src/*.o *.bin mydisk.qcow2 mydisk.raw