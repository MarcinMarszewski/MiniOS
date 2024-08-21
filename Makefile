C_SOURCES = $(wildcard src/*.c src/drivers/*.c src/cpu/*.c src/lib/*.c)
HEADERS = $(wildcard src/*.h src/drivers/*.h src/cpu/*.h src/lib/*.h)
OBJ = ${C_SOURCES:.c=.o src/cpu/interrupt.o}


CFLAGS = -g

fresh: clean run

fromdisk: clean os-image.bin mydisk.qcow2
	qemu-img convert -f qcow2 -O raw mydisk.qcow2 mydisk.raw
	dd if=os-image.bin of=mydisk.raw bs=512 conv=notrunc
#   qemu-img convert -f raw -O qcow2 mydisk.raw mydisk.qcow2
	qemu-system-i386 -hda mydisk.raw

setdisk:
	qemu-img create -f raw mydisk.raw 128M

os-image.bin: src/boot/bootsect.bin src/kernel.bin
	cat $^ > os-image.bin

src/kernel.bin: src/boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary


kernel.elf: boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ 

mydisk.qcow2:
	qemu-img create -f qcow2 mydisk.qcow2 128M

run: os-image.bin mydisk.qcow2
	qemu-system-i386 -hda mydisk.qcow2 -fda os-image.bin



%.o: %.c ${HEADERS}
	i686-elf-gcc ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf src/boot/*.bin src/boot/*.o src/cpu/*.o src/drivers/*.o src/lib/*.o src/*.o *.bin mydisk.qcow2 mydisk.raw