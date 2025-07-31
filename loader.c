#include "vm.h"

void load_program() {
    FILE *fp = fopen("vm/program.bin", "rb");
    if (!fp) {
        printf("Could not open program.bin\n");
        return;
    }

    fread(memory, 1, MEM_SIZE, fp);
    fclose(fp);
}
