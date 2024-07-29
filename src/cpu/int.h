#ifndef INT_H
#define INT_H

#define KERNEL_CS 0x08

extern void interrupt0();
extern void interrupt1();
extern void interrupt2();
extern void interrupt3();
extern void interrupt4();
extern void interrupt5();
extern void interrupt6();
extern void interrupt7();
extern void interrupt8();
extern void interrupt9();
extern void interrupt10();
extern void interrupt11();
extern void interrupt12();
extern void interrupt13();
extern void interrupt14();
extern void interrupt15();

typedef struct {
    unsigned short low_offset; /* Lower 16 bits of handler function address */
    unsigned short sel; /* Kernel segment selector */
    unsigned char always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    unsigned char flags; 
    unsigned short high_offset; /* Higher 16 bits of handler function address */
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

typedef struct {
   unsigned int ds; /* Data segment selector */
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   unsigned int int_no, err_code; /* Interrupt number and error code (if applicable) */
   unsigned int eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

void interrupt_install();
void interrupt_handler(registers_t r);

typedef void (*interrupt_t)(registers_t);
void register_interrupt_handler(unsigned char n, interrupt_t handler);

#endif