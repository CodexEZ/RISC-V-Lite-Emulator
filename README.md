# RISC-V Virtual Machine and Assembler

This project provides a simple virtual machine (VM) and a two-pass assembler for a subset of the RISC-V 32-bit integer instruction set (RV32I).

## Features

*   **RISC-V Core:** A 32-bit VM with 32 general-purpose registers.
*   **Two-Pass Assembler:** Translates RISC-V assembly into machine code, with support for forward-referencing labels.
*   **Dynamic Loading:** The VM loads a binary file at runtime, so no recompilation is needed to run different programs.
*   **Custom Instructions:** Includes `print` and `input` for basic I/O operations.

## Project Structure

-   `vm.c`, `vm.h`, `execute.c`, `loader.c`, `main.c`: Source code for the virtual machine.
-   `assembler.c`: Source code for the assembler.
-   `program.s`: An example assembly language program.
-   `program.bin`: The assembled binary of `program.s`.
-   `vm.exe`: The compiled virtual machine executable.
-   `assembler.exe`: The compiled assembler executable.

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

## Example Programs

Here are a few examples of programs you can write and run.

### Example 1: Simple Arithmetic

This program adds two numbers and prints the result.

```riscv
# program.s
addi x1, x0, 10   # Load 10 into register x1
addi x2, x0, 20   # Load 20 into register x2
add x3, x1, x2    # x3 = x1 + x2
print x3          # Print the value of x3
halt              # Stop the machine
```

### Example 2: User Input

This program asks the user for two numbers, adds them, and prints the sum.

```riscv
# program.s
input x1          # Get first number from user
input x2          # Get second number from user
add x3, x1, x2    # x3 = x1 + x2
print x3          # Print the sum
halt
```

### Example 3: Looping

This program asks for a number and then counts from 0 up to that number.

```riscv
# program.s
input x1          # Get the upper limit from the user
addi x2, x0, 0    # Initialize counter (x2) to 0
loop:
    print x2          # Print the current count
    addi x2, x2, 1    # Increment counter
    blt x2, x1, loop  # If x2 < x1, jump back to 'loop'
halt
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
- `blt rs1, rs2, label`
- `bge rs1, rs2, label`
- `bltu rs1, rs2, label`
- `bgeu rs1, rs2, label`

### Custom Instructions
- `halt`: Stops the VM execution.
- `print rd`: Prints the integer value of register `rd` to the console.
- `input rd`: Prompts the user for an integer and stores it in register `rd`.
