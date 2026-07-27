CC= i386-elf-gcc
LD= i386-elf-ld

C_SOURCES= $(wildcard src/*/*.c)
HEADERS= $(wildcard src/*/*.h)

BOOT_SOURCES= $(wildcard src/boot/*)
BOOT_OBJS= ${BOOT_SOURCES:.asm=.o} ${BOOT_SOURCES:.c=.o}

ASM_SOURCES= ${filter-out ${BOOT_SOURCES}, ${wildcard src/*/*.asm}}

OBJS= ${filter-out ${BOOT_OBJS}, ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o}}
BUILD_DIR= bin

C_FLAGS= -m32 -nostdlib -nostdinc -ffreestanding -fno-pie -fno-builtin-function -fno-builtin -static

KERNEL_LD_SCRIPT= kernel.ld
STAGE_2_LD_SCRIPT= stage_2.ld
LD_FLAGS= -static -s

all: dirs run

dirs:
	mkdir -p bin

clean:
	rm -rf bin/
	rm src/*/*.o

%.o: %.c ${HEADERS}
	${CC} ${C_FLAGS} -c $< -o $@ 

bin/%.o: src/boot/%.asm
	nasm -felf $< -o $@

src/%.o: src/%.asm
	nasm -felf $< -o $@

bin/%.bin: src/boot/%.asm
	nasm -fbin $< -o $@

bin/stage_2.bin: bin/boot_stage_2.o src/boot/stage_2.o
	${LD} -o $@ $^ --oformat binary ${LD_FLAGS} -T ${STAGE_2_LD_SCRIPT}

bin/kernel.bin: bin/start.o ${OBJS}
	${LD} -o $@ $^ --oformat binary ${LD_FLAGS} -T ${KERNEL_LD_SCRIPT}

bin/image.bin: bin/boot.bin bin/stage_2.bin bin/kernel.bin
	dd if=/dev/zero of=$@ bs=1M count=2
	mformat -i $@ -F
	mcopy -i $@ bin/stage_2.bin ::
	mcopy -i $@ bin/kernel.bin ::
	mcopy -s -i $@ ./root/* ::
	cat bin/boot.bin | dd of=$@ bs=1 count=512 conv=notrunc 

run: bin/image.bin
	rm src/*/*.o
	qemu-system-i386 -drive file=$<,format=raw
