#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LABELS 100
#define MAX_LINE_LEN 100

typedef struct {
    char name[MAX_LINE_LEN];
    int address;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;

int find_label(char* name) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            return labels[i].address;
        }
    }
    return -1;
}

uint32_t parse_reg(char* s) {
    if (s[0] == 'x') {
        return atoi(s + 1);
    }
    return atoi(s);
}

uint32_t assemble_r_type(char* mnemonic, char* rd_str, char* rs1_str, char* rs2_str) {
    uint32_t rd = parse_reg(rd_str);
    uint32_t rs1 = parse_reg(rs1_str);
    uint32_t rs2 = parse_reg(rs2_str);
    uint32_t opcode = 0x33;
    uint32_t funct3 = 0;
    uint32_t funct7 = 0;

    if (strcmp(mnemonic, "add") == 0) { funct3 = 0x0; funct7 = 0x00; }
    else if (strcmp(mnemonic, "sub") == 0) { funct3 = 0x0; funct7 = 0x20; }
    else if (strcmp(mnemonic, "sll") == 0) { funct3 = 0x1; funct7 = 0x00; }
    else if (strcmp(mnemonic, "xor") == 0) { funct3 = 0x4; funct7 = 0x00; }
    else if (strcmp(mnemonic, "srl") == 0) { funct3 = 0x5; funct7 = 0x00; }
    else if (strcmp(mnemonic, "sra") == 0) { funct3 = 0x5; funct7 = 0x20; }
    else if (strcmp(mnemonic, "or") == 0) { funct3 = 0x6; funct7 = 0x00; }
    else if (strcmp(mnemonic, "and") == 0) { funct3 = 0x7; funct7 = 0x00; }

    return (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t assemble_i_type(char* mnemonic, char* rd_str, char* rs1_str, char* imm_str) {
    uint32_t rd = parse_reg(rd_str);
    uint32_t rs1 = parse_reg(rs1_str);
    int32_t imm = atoi(imm_str);
    uint32_t opcode = 0x13;
    uint32_t funct3 = 0;
    uint32_t funct7 = 0;

    if (strcmp(mnemonic, "addi") == 0) { funct3 = 0x0; }
    else if (strcmp(mnemonic, "xori") == 0) { funct3 = 0x4; }
    else if (strcmp(mnemonic, "ori") == 0) { funct3 = 0x6; }
    else if (strcmp(mnemonic, "andi") == 0) { funct3 = 0x7; }
    else if (strcmp(mnemonic, "slli") == 0) { funct3 = 0x1; funct7 = 0x00; }
    else if (strcmp(mnemonic, "srli") == 0) { funct3 = 0x5; funct7 = 0x00; }
    else if (strcmp(mnemonic, "srai") == 0) { funct3 = 0x5; funct7 = 0x20; }
    else if (strcmp(mnemonic, "lw") == 0) { opcode = 0x03; funct3 = 0x2; }

    return (funct7 << 25) | (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
}

uint32_t assemble_s_type(char* mnemonic, char* rs2_str, char* imm_str, char* rs1_str) {
    uint32_t rs1 = parse_reg(rs1_str);
    uint32_t rs2 = parse_reg(rs2_str);
    int32_t imm = atoi(imm_str);
    uint32_t opcode = 0x23;
    uint32_t funct3 = 0x2;

    uint32_t imm_4_0 = imm & 0x1F;
    uint32_t imm_11_5 = (imm >> 5) & 0x7F;

    return (imm_11_5 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm_4_0 << 7) | opcode;
}

uint32_t assemble_b_type(char* mnemonic, char* rs1_str, char* rs2_str, char* label_name, int current_addr) {
    uint32_t rs1 = parse_reg(rs1_str);
    uint32_t rs2 = parse_reg(rs2_str);
    int label_addr = find_label(label_name);
    int32_t offset = label_addr - current_addr;
    uint32_t opcode = 0x63;
    uint32_t funct3 = 0;

    if (strcmp(mnemonic, "beq") == 0) { funct3 = 0x0; }
    else if (strcmp(mnemonic, "bne") == 0) { funct3 = 0x1; }
    else if (strcmp(mnemonic, "blt") == 0) { funct3 = 0x4; }
    else if (strcmp(mnemonic, "bge") == 0) { funct3 = 0x5; }
    else if (strcmp(mnemonic, "bltu") == 0) { funct3 = 0x6; }
    else if (strcmp(mnemonic, "bgeu") == 0) { funct3 = 0x7; }

    uint32_t imm_12 = (offset >> 12) & 0x1;
    uint32_t imm_10_5 = (offset >> 5) & 0x3F;
    uint32_t imm_4_1 = (offset >> 1) & 0xF;
    uint32_t imm_11 = (offset >> 11) & 0x1;

    return (imm_12 << 31) | (imm_10_5 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm_4_1 << 8) | (imm_11 << 7) | opcode;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input.s> <output.bin>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    // First pass: find labels
    char line[MAX_LINE_LEN];
    int address = 0;
    while (fgets(line, sizeof(line), in)) {
        char line_copy[MAX_LINE_LEN];
        strcpy(line_copy, line);
        char* token = strtok(line_copy, " \t\n");

        if (!token) continue; // Skip empty line

        int has_instruction = 0;
        if (token[strlen(token) - 1] == ':') {
            // It's a label
            token[strlen(token) - 1] = '\0';
            strcpy(labels[label_count].name, token);
            labels[label_count].address = address;
            label_count++;
            
            // Check for instruction on the same line
            token = strtok(NULL, " \t\n");
            if (token) {
                has_instruction = 1;
            }
        } else {
            // Not a label, so it must be an instruction
            has_instruction = 1;
        }
        
        if (has_instruction) {
            address += 4;
        }
    }

    fseek(in, 0, SEEK_SET);
    FILE *out = fopen(argv[2], "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    // Second pass: assemble
    address = 0;
    while (fgets(line, sizeof(line), in)) {
        char* mnemonic = strtok(line, " \t\n,()");
        if (!mnemonic) {
            continue; // Skip empty lines
        }

        // If the line starts with a label, skip the label part
        if (mnemonic[strlen(mnemonic) - 1] == ':') {
            mnemonic = strtok(NULL, " \t\n,()");
            if (!mnemonic) {
                continue; // Line only contained a label
            }
        }

        uint32_t instruction = 0;
        if (strcmp(mnemonic, "add") == 0 || strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "sll") == 0 || strcmp(mnemonic, "xor") == 0 || strcmp(mnemonic, "srl") == 0 || strcmp(mnemonic, "sra") == 0 || strcmp(mnemonic, "or") == 0 || strcmp(mnemonic, "and") == 0) {
            char* rd = strtok(NULL, " \t\n,()");
            char* rs1 = strtok(NULL, " \t\n,()");
            char* rs2 = strtok(NULL, " \t\n,()");
            instruction = assemble_r_type(mnemonic, rd, rs1, rs2);
        } else if (strcmp(mnemonic, "addi") == 0 || strcmp(mnemonic, "xori") == 0 || strcmp(mnemonic, "ori") == 0 || strcmp(mnemonic, "andi") == 0 || strcmp(mnemonic, "slli") == 0 || strcmp(mnemonic, "srli") == 0 || strcmp(mnemonic, "srai") == 0) {
            char* rd = strtok(NULL, " \t\n,()");
            char* rs1 = strtok(NULL, " \t\n,()");
            char* imm = strtok(NULL, " \t\n,()");
            instruction = assemble_i_type(mnemonic, rd, rs1, imm);
        } else if (strcmp(mnemonic, "lw") == 0) {
            char* rd = strtok(NULL, " \t\n,()");
            char* imm = strtok(NULL, " \t\n,()");
            char* rs1 = strtok(NULL, " \t\n,()");
            instruction = assemble_i_type(mnemonic, rd, rs1, imm);
        } else if (strcmp(mnemonic, "sw") == 0) {
            char* rs2 = strtok(NULL, " \t\n,()");
            char* imm = strtok(NULL, " \t\n,()");
            char* rs1 = strtok(NULL, " \t\n,()");
            instruction = assemble_s_type(mnemonic, rs2, imm, rs1);
        } else if (strcmp(mnemonic, "beq") == 0 || strcmp(mnemonic, "bne") == 0 || strcmp(mnemonic, "blt") == 0 || strcmp(mnemonic, "bge") == 0 || strcmp(mnemonic, "bltu") == 0 || strcmp(mnemonic, "bgeu") == 0) {
            char* rs1 = strtok(NULL, " \t\n,()");
            char* rs2 = strtok(NULL, " \t\n,()");
            char* label = strtok(NULL, " \t\n,()");
            instruction = assemble_b_type(mnemonic, rs1, rs2, label, address);
        } else if (strcmp(mnemonic, "halt") == 0) {
            instruction = 0x00000000;
        } else if (strcmp(mnemonic, "print") == 0) {
            char* rd = strtok(NULL, " \t\n,()");
            instruction = (parse_reg(rd) << 7) | 0x0B;
        } else if (strcmp(mnemonic, "input") == 0) {
            char* rd = strtok(NULL, " \t\n,()");
            instruction = (parse_reg(rd) << 7) | 0x0C;
        }

        if (instruction != 0) {
            fwrite(&instruction, sizeof(instruction), 1, out);
            address += 4;
        }
    }

    fclose(in);
    fclose(out);

    return 0;
}
