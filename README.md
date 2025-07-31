# RISC-V Virtual Machine and Assembler

This project provides a simple virtual machine (VM) and an assembler for a subset of the RISC-V instruction set architecture.

## Project Structure

- `vm.c`, `vm.h`, `execute.c`, `loader.c`, `main.c`: Source code for the virtual machine.
- `assembler.c`: Source code for the assembler.
- `program.s`: An example assembly language program.
- `program.bin`: The assembled binary of `program.s`.
- `vm.exe`: The compiled virtual machine executable.
- `assembler.exe`: The compiled assembler executable.

## Compilation

You can compile the virtual machine and the assembler using a C compiler like GCC.

### Compiling the VM

```bash
gcc -o vm/vm.exe vm/main.c vm/vm.c vm/execute.c vm/loader.c
```

### Compiling the Assembler

```bash
gcc -o vm/assembler.exe vm/assembler.c
```

## Usage

The workflow consists of two main steps: assembling your assembly code into a binary file and then running that binary file with the VM.

### 1. Assembling a Program

The assembler takes an assembly file (`.s`) as input and produces a binary file (`.bin`) as output.

**Syntax:**

```bash
./vm/assembler.exe <input_file.s> <output_file.bin>
```

**Example:**

To assemble the provided `program.s` file into `program.bin`:

```bash
./vm/assembler.exe vm/program.s vm/program.bin
```

### 2. Running the VM

The VM is hardcoded to load and execute a file named `program.bin` from the `vm/` directory.

**To run the VM:**

```bash
./vm/vm.exe
```

The VM will then execute the instructions from `vm/program.bin`.

## Example Workflow

1.  **Write your assembly code:** Create or edit a `.s` file (e.g., `my_program.s`).
2.  **Assemble it:**
    ```bash
    ./vm/assembler.exe vm/my_program.s vm/program.bin
    ```
    *Note: The output file must be named `program.bin` and placed in the `vm/` directory for the VM to find it.*
3.  **Run the VM:**
    ```bash
    ./vm/vm.exe
    ```

## Supported Instructions

The assembler and VM support the following instructions:

### R-type
- `add rd, rs1, rs2`
- `sub rd, rs1, rs2`
- `sll rd, rs1, rs2`
- `xor rd, rs1, rs2`
- `srl rd, rs1, rs2`
- `sra rd, rs1, rs2`
- `or rd, rs1, rs2`
- `and rd, rs1, rs2`

### I-type
- `addi rd, rs1, imm`
- `xori rd, rs1, imm`
- `ori rd, rs1, imm`
- `andi rd, rs1, imm`
- `slli rd, rs1, imm`
- `srli rd, rs1, imm`
- `srai rd, rs1, imm`
- `lw rd, imm(rs1)`

### S-type
- `sw rs2, imm(rs1)`

### B-type
- `beq rs1, rs2, label`
- `bne rs1, rs2, label`

### Custom Instructions
- `halt`: Stops the VM execution.
- `print rd`: Prints the integer value of register `rd` to the console.
- `input rd`: Prompts the user for an integer and stores it in register `rd`.
