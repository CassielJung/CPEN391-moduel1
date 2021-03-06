/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "sys/alt_stdio.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "sys/alt_irq.h"


/* Function to compute primes upto 0xFFFFFFFF (Max value for 32 bit int)
 * primeList[] store prime numbers upto 0xFFFF (Max value for 16 bit int)
 *     - size 6543 is the approximate number of primes upto 0xFFFF
 */
void computePrime() {
  // initialize primeList
  uint32_t primeList[6543];
  primeList[0] = 2;
  primeList[1] = 3;

  uint32_t numPrime = 2;
  bool isPrime;

  for (uint32_t i = 5; i < 0xFFFFFFFF; i=i+2) {
      isPrime = true; // reset isPrime
    for (uint32_t j = 0; j < numPrime; j++) {
      if (i % primeList[j] == 0) { // if i is divisible by one of previous prime
        isPrime = false;           // i is not a prime number
        break;
      }
    }

    if (isPrime) {                 // if i was a prime
      if (numPrime < 6543) {
        numPrime++;                // increment numPrime
        primeList[numPrime-1] = i; // save to primeList
      }
      // send calculated prime number to memory
      while(!(IORD_ALTERA_AVALON_PIO_DATA(PRIME_NUMBER_BASE) & 0x00000001));
      IOWR_ALTERA_AVALON_PIO_DATA(PRIME_NUMBER_BASE, i);
    }

  }
}


/* global variables used to light LED when interrupt made */
int ledPattern[5][20] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},    // 0% - brightness level 0
                         {1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},    // 20% - brightness level 1
                         {1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},    // 40% - brightness level 2
                         {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0},    // 60% - brightness level 3
                         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};   // 100% - brightness level 4

int brightness;   // the brightness level (row)
int direction;    // when 1 brightness level increase and when 0 brightness level decrease
int counter;      // count timer interrupt to change brightness level every 200ms

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void timer_ISR(void* context)
#else
void timer_ISR(void* context, alt_u32 id)
#endif
{
  if (ledPattern[brightness][counter]) {              // if ledPattern[][] == 1
    IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x3FF);    // light up all the LED
  } else {
    IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0x0);      // else turn off all the LED
  }

  counter++;

  if (counter == 20) {
    counter = 0;

    // brightness pattern: 0-1-2-3-4-4-3-2-1-0
    if (direction) brightness++;
    else brightness--;

    if (brightness > 4) {
      direction = 0; brightness--;
    }
    if (brightness < 0) {
      direction = 1; brightness++;
    }
  }

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0); // reset timer
}

int main()
{
  // Variable Initialization
  brightness = 0; counter = 0; direction = 1;

  /* Timer Initialization */
  // System clock: 50MHz = 50,000,000 clock cycle per second
  // 10ms = 0.01s = 0.5MHZ = 500,000(0x7A120) clock cycle
  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0007); // set higher 16 bits of period
  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0xA120); // set lower 16 bits of period

  // Clear IRQ status
  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

  // enable interrupt
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, ALTERA_AVALON_TIMER_CONTROL_CONT_MSK
                                                 | ALTERA_AVALON_TIMER_CONTROL_ITO_MSK
												                         | ALTERA_AVALON_TIMER_CONTROL_START_MSK);

  // Register the ISR for timer event
  #ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ, timer_ISR, 0, 0x0);
  #else
    alt_ic_isr_register(TIMER_IRQ, 0, timer_ISR);
  #endif


  /* Event loop never exits. */
  while (1) {
    computePrime();
  };

  return 0;
}
