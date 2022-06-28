#include <string.h>
#include "gd32vf103xb.h"

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void delay_ms(uint32_t d) {
  for (uint32_t i = 0; i < (d * 1300); i++) {
    __asm__( "nop; nop; nop" );
  }
}

// Longan Nano / LILYGO
// PC13 -- red   led
// PA1  -- green led
// PA2  -- blue  led

int main () {
  memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) ); // copy Flash -> RAM
  memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) ); // Clear .bss RAM

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  // low speed output
  GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
  GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

  // push-pull
  GPIOA->CRL |= (0x2 << GPIO_CRL_MODE1_Pos);
  GPIOA->CRL |= (0x2 << GPIO_CRL_MODE2_Pos);

  while(1) {
    for (int i = 0; i < 4; i++) {
      GPIOA->ODR = i << 1; // green / blue mix
      delay_ms(1000);
    }
  }
  return 0;
}
