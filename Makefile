TOOLCHAIN = riscv64-linux-gnu
CC        = $(TOOLCHAIN)-gcc
CFLAGS    = -g -o0 -ffreestanding

all: hello.elf

hello.o: hello.c
	$(CC) -c $(CFLAGS) -o hello.o hello.c
	$(TOOLCHAIN)-size $@

start.o: start.s
	$(TOOLCHAIN)-as -g -o start.o start.s
	$(TOOLCHAIN)-size $@

hacl-hmac.o: hacl-c/Hacl_HMAC_SHA2_256.c
	$(CC) -c $(CFLAGS) -o $@ $<
	$(TOOLCHAIN)-size $@

hello.elf: hello.o start.o baremetal.ld hacl-hmac.o
	$(TOOLCHAIN)-ld -T baremetal.ld -o hello.elf hello.o start.o hacl-hmac.o
	$(TOOLCHAIN)-size $@

run: hello.elf
	@echo "Ctrl-A C for QEMU console, then quit to exit"
	qemu-system-riscv64 -nographic -serial mon:stdio -machine virt -bios hello.elf

clean:
	rm -f hello.o start.o hello.elf *.o *.elf
