// main.c
#include "vm.h"

void load_program();
void execute_instruction(uint32_t instr);
void init_vm();

int main() {
    init_vm();
    load_program();

    while (cpu.running) {
        uint32_t instr = memory[cpu.pc] | (memory[cpu.pc + 1] << 8) | (memory[cpu.pc + 2] << 16) | (memory[cpu.pc + 3] << 24);
        execute_instruction(instr);
    }

    return 0;
}
