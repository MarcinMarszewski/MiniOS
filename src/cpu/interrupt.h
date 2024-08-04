#ifndef INTERRUPT_H
#define INTERRUPT_H

#define KERNEL_CS 0x08

typedef struct {
    unsigned short low_offset;
    unsigned short sel;
    unsigned char always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    unsigned char flags; 
    unsigned short high_offset;
} __attribute__((packed)) interrupt_gate_t ;

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) interrupt_register_t;

#define INTERRUPT_ENTRIES 256
interrupt_gate_t interrupts[INTERRUPT_ENTRIES];
interrupt_register_t interrupt_reg;

void set_interrupt_gate(int n, unsigned int handler);
void set_interrupt();
void set_interrupt_ports();
void assign_interrupts();

#endif