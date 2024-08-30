#ifndef TIMER_H
#define TIMER_H

#include "../cpu/isr.h"

#define PIT_FREQUENCY 1193180

extern unsigned int __ticks_elapsed;

void set_timer_frequency(unsigned int frequency);
void timer_irq(registers_t r);

#endif