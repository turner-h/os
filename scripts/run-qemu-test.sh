timeout 60 qemu-system-i386 \
  -drive format=raw,file=bin/image.bin \
  -display none -serial file:bin/serial.log \
  -device isa-debug-exit,iobase=0xf4,iosize=0x04 \
  -no-reboot -d int,cpu_reset -D bin/qemu.log
status=$?
cat bin/serial.log
[ "$status" -eq 33 ]
