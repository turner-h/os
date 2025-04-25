CC= i386-elf-gcc
LD= i386-elf-ld

C_SOURCES= $(wildcard src/kernel/*.c src/cpu/*.c src/drivers/*.c src/lib/*.c)
HEADERS= $(wildcard src/kernel/*.h src/cpu/*.h src/drivers/*.h src/lib/*.h)

BOOT_SOURCES= $(wildcard src/boot/*.asm)

ASM_SOURCES= ${filter-out ${BOOT_SOURCES}, ${wildcard src/kernel/*.asm src/cpu/*.asm src/drivers/*.asm src/lib/*.asm}}

OBJS= ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o}
BUILD_DIR= bin

C_FLAGS= -m32 -nostdlib -nostdinc -ffreestanding -fno-pie -fno-builtin-function -fno-builtin -static
LD_FLAGS= -static -s

all: dirs run

dirs:
	mkdir -p bin

clean:
	rm -rf bin/
	rm src/kernel/*.o src/cpu/*.o src/drivers/*.o src/lib/*.o

%.o: %.c ${HEADERS}
	${CC} ${C_FLAGS} -c $< -o $@ 

bin/%.o: src/boot/%.asm
	nasm -felf $< -o $@

src/%.o: src/%.asm
	nasm -felf $< -o $@

bin/%.bin: src/boot/%.asm
	nasm -fbin $< -o $@

bin/kernel.bin: bin/start.o ${OBJS}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary ${LD_FLAGS}

bin/image.bin: bin/boot.bin bin/kernel.bin
	cat $^ > $@

run: bin/image.bin
	qemu-system-i386-unsigned -drive file=$<,format=raw
	rm src/kernel/*.o src/cpu/*.o src/drivers/*.o src/lib/*.o