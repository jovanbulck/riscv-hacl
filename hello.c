#include <stdint.h>
#include "hacl-c/Hacl_HMAC_SHA2_256.h"

#define UART0_BASE 0x10000000

// Use a datasheet for a 16550 UART
// For example: https://www.ti.com/lit/ds/symlink/tl16c550d.pdf
#define REG(base, offset) ((*((volatile unsigned char *)(base + offset))))
#define UART0_DR    REG(UART0_BASE, 0x00)
#define UART0_FCR   REG(UART0_BASE, 0x02)
#define UART0_LSR   REG(UART0_BASE, 0x05)
																						
#define UARTFCR_FFENA 0x01                // UART FIFO Control Register enable bit
#define UARTLSR_THRE 0x20                 // UART Line Status Register Transmit Hold Register Empty bit
#define UART0_FF_THR_EMPTY (UART0_LSR & UARTLSR_THRE)

void uart_putc(char c) {
  while (!UART0_FF_THR_EMPTY);            // Wait until the FIFO holding register is empty
  UART0_DR = c;                           // Write character to transmitter register
}

void uart_puts(const char *str) {
  while (*str) {                          // Loop until value at string pointer is zero
    uart_putc(*str++);                    // Write the character and increment pointer
  }
}


void* memset(void* ptr, int len, size_t val)
{
  char *buf = (char*) ptr;
  int i = 0;
  for (i=0; i<len; i++)
        buf[i] = val;
  return ptr;
}

void* memcpy(void*__restrict dst, const void*__restrict src, size_t size)
{
  char *out = (char*) dst;
  char *in = (char*) src;
  int i = 0;
  for (i=0; i<size; i++)
        out[i] = in[i];
  return dst;
}

/* https://github.com/llvm-mirror/compiler-rt/blob/master/lib/builtins/bswapsi2.c */
uint32_t __bswapsi2(uint32_t u) {
  return ((((u)&0xff000000) >> 24) |
          (((u)&0x00ff0000) >> 8)  |
          (((u)&0x0000ff00) << 8)  |
          (((u)&0x000000ff) << 24));
}

/* https://github.com/llvm-mirror/compiler-rt/blob/master/lib/builtins/bswapdi2.c */
uint64_t __bswapdi2(uint64_t u) {
  return (
      (((u)&0xff00000000000000ULL) >> 56) |
      (((u)&0x00ff000000000000ULL) >> 40) |
      (((u)&0x0000ff0000000000ULL) >> 24) |
      (((u)&0x000000ff00000000ULL) >> 8)  |
      (((u)&0x00000000ff000000ULL) << 8)  |
      (((u)&0x0000000000ff0000ULL) << 24) |
      (((u)&0x000000000000ff00ULL) << 40) |
      (((u)&0x00000000000000ffULL) << 56));
}

void dump_hex(char *str, uint8_t* buf, size_t length) {
    uart_puts(str);
    uart_puts(": ");
    for (int i = 0; i < length; i++)
    {
        uint8_t byte = buf[i];
        uart_putc((byte >> 4) < 10 ? (byte >> 4) + '0' : (byte >> 4) - 10 + 'A');
        uart_putc((byte & 0x0F) < 10 ? (byte & 0x0F) + '0' : (byte & 0x0F) - 10 + 'A');
    }
    uart_puts("\n");
}

#define MSG_LEN         13
#define KEY_LEN         64
#define MAC_LEN         32

char *msg = "hello, world!";
uint8_t key[KEY_LEN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
};
uint8_t mac[MAC_LEN] = {0};

void main()
{
  UART0_FCR = UARTFCR_FFENA;              // Set the FIFO for polled operation

  dump_hex("msg", msg, MSG_LEN);
  dump_hex("key", key, KEY_LEN);

  hmac(mac, key, KEY_LEN, msg, MSG_LEN);

  dump_hex("mac", mac, MAC_LEN);

  while (1);                              // Loop forever to prevent program from ending
}
