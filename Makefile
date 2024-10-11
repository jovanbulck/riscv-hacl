TOOLCHAIN = riscv64-linux-gnu

default: hello.elf

hello.o: hello.c
	$(TOOLCHAIN)-gcc -c -g -O0 -ffreestanding -o hello.o hello.c

start.o: start.s
	$(TOOLCHAIN)-as -g -o start.o start.s

hello.elf: hello.o start.o baremetal.ld
	$(TOOLCHAIN)-ld -T baremetal.ld -o hello.elf hello.o start.o

run: hello.elf
	@echo "Ctrl-A C for QEMU console, then quit to exit"
	qemu-system-riscv64 -nographic -serial mon:stdio -machine virt -bios hello.elf

clean:
	rm -f hello.o start.o hello.elf
