#include "vm.h"

void execute_instruction(uint32_t instr) {
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;

    // I-type immediate
    int32_t imm_i = (int32_t)instr >> 20;

    // S-type immediate
    int32_t imm_s = ((int32_t)(instr & 0xFE000000) >> 20) | ((instr >> 7) & 0x1F);

    // B-type immediate
    int32_t imm_b = ((int32_t)(instr & 0x80000000) >> 19) | ((instr & 0x80) << 4) | ((instr >> 20) & 0x7E0) | ((instr >> 7) & 0x1E);


    switch (opcode) {
        case 0x03: // LW
            if (funct3 == 0x2) {
                uint32_t addr = cpu.reg[rs1] + imm_i;
                cpu.reg[rd] = (memory[addr] << 24) | (memory[addr+1] << 16) | (memory[addr+2] << 8) | memory[addr+3];
            }
            break;
        case 0x23: // SW
            if (funct3 == 0x2) {
                uint32_t addr = cpu.reg[rs1] + imm_s;
                memory[addr]   = (cpu.reg[rs2] >> 24) & 0xFF;
                memory[addr+1] = (cpu.reg[rs2] >> 16) & 0xFF;
                memory[addr+2] = (cpu.reg[rs2] >> 8) & 0xFF;
                memory[addr+3] = cpu.reg[rs2] & 0xFF;
            }
            break;
        case 0x13: // I-type instructions: ADDI, XORI, ORI, ANDI, SLLI, SRLI, SRAI
            switch (funct3) {
                case 0x0: // ADDI
                    cpu.reg[rd] = cpu.reg[rs1] + imm_i;
                    break;
                case 0x4: // XORI
                    cpu.reg[rd] = cpu.reg[rs1] ^ imm_i;
                    break;
                case 0x6: // ORI
                    cpu.reg[rd] = cpu.reg[rs1] | imm_i;
                    break;
                case 0x7: // ANDI
                    cpu.reg[rd] = cpu.reg[rs1] & imm_i;
                    break;
                case 0x1: // SLLI
                    cpu.reg[rd] = cpu.reg[rs1] << (imm_i & 0x1F);
                    break;
                case 0x5: // SRLI, SRAI
                    if ((imm_i >> 5) == 0x00) { // SRLI
                        cpu.reg[rd] = cpu.reg[rs1] >> (imm_i & 0x1F);
                    } else { // SRAI
                        cpu.reg[rd] = (int32_t)cpu.reg[rs1] >> (imm_i & 0x1F);
                    }
                    break;
            }
            break;
        case 0x33: // R-type instructions: ADD, SUB, SLL, XOR, SRL, SRA, OR, AND
            switch (funct3) {
                case 0x0: // ADD, SUB
                    if (funct7 == 0x00) { // ADD
                        cpu.reg[rd] = cpu.reg[rs1] + cpu.reg[rs2];
                    } else { // SUB
                        cpu.reg[rd] = cpu.reg[rs1] - cpu.reg[rs2];
                    }
                    break;
                case 0x1: // SLL
                    cpu.reg[rd] = cpu.reg[rs1] << (cpu.reg[rs2] & 0x1F);
                    break;
                case 0x4: // XOR
                    cpu.reg[rd] = cpu.reg[rs1] ^ cpu.reg[rs2];
                    break;
                case 0x5: // SRL, SRA
                    if (funct7 == 0x00) { // SRL
                        cpu.reg[rd] = cpu.reg[rs1] >> (cpu.reg[rs2] & 0x1F);
                    } else { // SRA
                        cpu.reg[rd] = (int32_t)cpu.reg[rs1] >> (cpu.reg[rs2] & 0x1F);
                    }
                    break;
                case 0x6: // OR
                    cpu.reg[rd] = cpu.reg[rs1] | cpu.reg[rs2];
                    break;
                case 0x7: // AND
                    cpu.reg[rd] = cpu.reg[rs1] & cpu.reg[rs2];
                    break;
            }
            break;
        case 0x63: // BEQ, BNE
            if (funct3 == 0x0) { // BEQ
                if (cpu.reg[rs1] == cpu.reg[rs2]) {
                    cpu.pc += imm_b;
                    return;
                }
            } else if (funct3 == 0x1) { // BNE
                if (cpu.reg[rs1] != cpu.reg[rs2]) {
                    cpu.pc += imm_b;
                    return;
                }
            }
            break;
        case 0x6F: // JAL
            cpu.reg[rd] = cpu.pc + 4;
            cpu.pc += ((int32_t)(instr & 0x80000000) >> 11) | (instr & 0xFF000) | ((instr >> 9) & 0x800) | ((instr >> 20) & 0x7FE);
            return;
        case 0x00: // HALT (custom instruction)
            if (instr == 0x00000000) {
                cpu.running = 0;
                return;
            }
            break;
        case 0x0B: // Custom PRINT instruction
            printf("x%d = %d\n", rd, cpu.reg[rd]);
            break;
        case 0x0C: // Custom INPUT instruction
            printf("Enter a value for x%d: ", rd);
            scanf("%d", &cpu.reg[rd]);
            break;
        default:
            printf("Unknown opcode: %X\n", opcode);
            cpu.running = 0;
            return;
    }

    cpu.pc += 4;
}
