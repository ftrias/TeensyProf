/************

Copyright 2019 by Fernando Trias. All rights reserved.

Strategy:

1. Create a table to store the hit counts
2. Take over systick and use it to sample the current location
3. In the new systick, add 1 to the table of hit counts
4. When end is called, write out the table

************/

#include <Arduino.h>
#include "TeensyProf.h"

// from startup
extern void (* _VectorsRam[])(void);
extern "C" void systick_isr(void);
auto save_isr = _VectorsRam[15]; 

// from the linker file
extern const uint32_t _stext;
extern const uint32_t _etext;

// the lowest and highest program addresses
const uint32_t lowpc = (const uint32_t)&_stext;
const uint32_t highpc = (const uint32_t)&_etext;

int scale = 4; // shift addresses by this number
#define ADDR2IDX(addr) ((addr-lowpc)>>scale)
#define IDX2ADDR(idx) ((idx<<scale)+lowpc)
const int slots = ADDR2IDX(highpc);
const int textsize = highpc - lowpc;

// this is the hit counter table
uint16_t *counter;

// The stack frame saved by interrupt
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} interrupt_stack_t;
interrupt_stack_t *fault_reg;

void __attribute__((noinline, no_instrument_function)) gprof_systick_process(void) {
  static uint32_t pc, idx; // make static to avoid using stack variables
  if (counter) {
    pc = fault_reg->pc;
    if (pc >= lowpc && pc < highpc) {
      idx = ADDR2IDX(pc);
      counter[idx]++;
    }
  }
}

__attribute__((naked, noinline, no_instrument_function)) 
void gprof_systick_isr(void) {
  register interrupt_stack_t *rsp __asm("sp");
  fault_reg = rsp;
  __asm__ volatile("push {r0-r4,lr}");
  gprof_systick_process();
  save_isr(); /* call normal SysTick handler */
  __asm__ volatile("pop {r0-r4,lr}");
  __asm__ volatile ("bx lr");
}

/* Stop profiling to the profiling buffer pointed to by p. */
__attribute__((no_instrument_function)) 
int profile_off () {
  _VectorsRam[15] = save_isr;
  return 0;
}

/* Create a timer thread and pass it a pointer P to the profiling buffer. */
__attribute__((no_instrument_function)) 
int profile_on () {
  if (counter) return -2; // already running!
  counter = (uint16_t*)calloc(slots, sizeof(counter[0]));
  if (! counter) return -1; // out of memory
  save_isr = _VectorsRam[15];
  _VectorsRam[15] = gprof_systick_isr;
  return 0;
}

extern "C" bool TeensyProf_begin() {
  if (profile_on() == 0) return true;
  return false;
}

/*
 * For now, we use the old BSD-style format because it is
 * the most compatible and easiest to implement.
 */

struct gmon_hdr {
        uint32_t lpc;            /* base pc address of sample buffer */
        uint32_t hpc;            /* max pc address of sampled buffer */
        uint32_t ncnt;           /* size of sample buffer (plus this header) */
        uint32_t version;        /* version number */
        uint32_t profrate;       /* profiling clock rate */
        uint32_t spare[3];       /* reserved */
};

#define GMONVERSION     0x00051879

void TeensyProf_end() {
  profile_off();
  gmon_hdr h;
  h.lpc = lowpc;
  h.hpc = highpc;
  h.ncnt = slots * sizeof(counter[0]) + sizeof(h);
  h.version = GMONVERSION;
  h.profrate = 1000;
  int fp = TeensyProf_open("gmon.out", 0, 0);
  TeensyProf_write(fp, &h, sizeof(h));
  TeensyProf_write(fp, counter, slots * sizeof(counter[0]));
  TeensyProf_close(fp);
  free(counter);
  counter = NULL;
}
