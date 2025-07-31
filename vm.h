#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdio.h>

#define MEM_SIZE 0x10000

typedef struct {
    uint32_t reg[32];
    uint32_t pc;
    uint8_t zero;     // Z flag
    uint8_t carry;    // C flag (for future use)
    uint8_t sign;     // S flag (MSB of result)
    uint8_t running;
} CPU;


extern CPU cpu;
extern uint8_t memory[MEM_SIZE];

#endif // VM_H
