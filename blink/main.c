#include <stdint.h>
#include "gd32vf103.h"

void memCpy(void *dest, void *src, unsigned int len) {
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
   for (unsigned int i = 0; i < len; i++) {
    cdest[i] = csrc[i];
   }
}

void *memSet(void *s, int c,  unsigned int len) {
  unsigned char* p=s;
  while(len--) {
    *p++ = (unsigned char)c;
  }
  return s;
}

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

// Simple 'busy loop' delay method.
void delay_cycles( uint32_t cyc ) {
  uint32_t d_i;
  for ( d_i = 0; d_i < cyc; ++d_i ) {
    __asm__( "nop" );
  }
}

// 'main' method which gets called from the boot code.
int main () {
  // Copy initialized data from .sidata (Flash) to .data (RAM)
  memCpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
  // Clear the .bss RAM section.
  memSet( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  // low speed output
  GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

  // push-pull
  GPIOA->CRL |= (0x2 << GPIO_CRL_MODE1_Pos);
  GPIOA->CRL |= (0x2 << GPIO_CRL_MODE2_Pos);

  while(1) {
    GPIOA->ODR = 2; // blue LED on
    delay_cycles(200000);
    GPIOA->ODR = 6; // blue LED off
    delay_cycles(1000000);
  }
  return 0;
}
