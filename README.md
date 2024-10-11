# Bare-metal RISC-V programming with HACL* cryptographic library

Example usage:

```
jo@gropius:~/Documents/riscv-helloworld-c$ make run 
riscv64-linux-gnu-gcc -c -g -o0 -ffreestanding -o hello.o hello.c
riscv64-linux-gnu-size hello.o
   text	   data	    bss	    dec	    hex	filename
   1026	     72	     32	   1130	    46a	hello.o
riscv64-linux-gnu-as -g -o start.o start.s
riscv64-linux-gnu-size start.o
   text	   data	    bss	    dec	    hex	filename
     20	  65536	      0	  65556	  10014	start.o
riscv64-linux-gnu-gcc -c -g -o0 -ffreestanding -o hacl-hmac.o hacl-c/Hacl_HMAC_SHA2_256.c
riscv64-linux-gnu-size hacl-hmac.o
   text	   data	    bss	    dec	    hex	filename
   4738	      0	      0	   4738	   1282	hacl-hmac.o
riscv64-linux-gnu-ld -T baremetal.ld -o hello.elf hello.o start.o hacl-hmac.o
riscv64-linux-gnu-size hello.elf
   text	   data	    bss	    dec	    hex	filename
   5586	 131072	     32	 136690	  215f2	hello.elf
Ctrl-A C for QEMU console, then quit to exit
qemu-system-riscv64 -nographic -serial mon:stdio -machine virt -bios hello.elf
msg: 68656C6C6F2C20776F726C6421
key: 000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F
mac: AD2437ADF9EA20A8F65545782616830B89B719EA9A62604E4889430194F842B1
QEMU: Terminated
jo@gropius:~/Documents/riscv-helloworld-c$ echo -n "hello, world!" | openssl dgst -sha256 -mac hmac -macopt hexkey:000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F
SHA2-256(stdin)= ad2437adf9ea20a8f65545782616830b89b719ea9a62604e4889430194f842b1
jo@gropius:~/Documents/riscv-helloworld-c$ 
```

