#include "vm.h"

CPU cpu;
uint8_t memory[MEM_SIZE];

void init_vm(){
    cpu.pc = 0x0000;
    cpu.running = 1;
    for (int i=0; i<32 ; i++) cpu.reg[i] = 0;
    for (int i = 0; i < MEM_SIZE; i++) memory[i] = 0;
}
