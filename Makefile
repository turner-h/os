CC= i386-elf-gcc
LD= i386-elf-ld

C_SOURCES= $(wildcard src/*.c)
HEADERS= $(wildcard src/*.h)

OBJS= ${C_SOURCES:.c=.o}
BUILD_DIR= bin

C_FLAGS= -m32 -nostdlib -nostdinc -ffreestanding -fno-pie -fno-builtin-function -fno-builtin -static
LD_FLAGS= -static -s

all: dirs run

dirs:
	mkdir -p bin

clean:
	rm -rf bin/
	rm src/*.o

%.o: %.c ${HEADERS}
	${CC} ${C_FLAGS} -c $< -o $@ 

${BUILD_DIR}/%.o: src/%.asm
	nasm -felf $< -o $@

bin/%.bin: src/%.asm
	nasm -fbin $< -o $@

bin/kernel.bin: bin/start.o ${OBJS}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary ${LD_FLAGS}

bin/image.bin: bin/boot.bin bin/kernel.bin
	cat $^ > $@

run: bin/image.bin
	qemu-system-i386-unsigned -drive file=$<,format=raw
	rm src/*.o