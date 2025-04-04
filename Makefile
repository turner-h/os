all: dirs run

dirs:
	mkdir -p bin

clean:
	rm -rf bin/

bin/main.o: src/main.c
	i386-elf-gcc -ffreestanding $< -o $@ 

bin/start.o: src/start.asm
	nasm -felf $< -o $@

bin/kernel.bin: bin/start.o bin/main.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary 

bin/boot.bin: src/boot.asm
	nasm -fbin $< -o $@

bin/image.bin: bin/boot.bin bin/kernel.bin
	cat $^ > $@

run: bin/image.bin
	qemu-system-i386-unsigned $<