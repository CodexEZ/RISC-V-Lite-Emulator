input x1
addi x2, x0, 0
loop:
print x2
addi x2, x2, 1
blt x2, x1, loop
halt
