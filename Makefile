C_SOURCES = $(wildcard src/*.c src/drivers/*.c src/cpu/*.c)
HEADERS = $(wildcard src/*.h src/drivers/*.h src/cpu/*.h)
#OBJ = ${C_SOURCES:.c=.o src/cpu/interrupt.o}
OBJ = ${C_SOURCES:.c=.o}


CFLAGS = -g

fresh: clean run

os-image.bin: src/boot/bootsect.bin src/kernel.bin
	cat $^ > os-image.bin

src/kernel.bin: src/boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary


kernel.elf: boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda os-image.bin



%.o: %.c ${HEADERS}
	i686-elf-gcc ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf src/boot/*.bin src/boot/*.o src/cpu/*.o src/drivers/*.o src/lib/*.o src/*.o *.bin