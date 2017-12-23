// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// set first factor x to R0
@R0
D=M
@x
M=D

// set second factor y to R1
@R1
D=M
@y
M=D

// set product R2 to 0
@0
D=A
@R2
M=D

(WHILE)

// loop condition
@x
D=M // set D to x
@END
D;JLE // go to END if x <= 0

// loop body
@y
D=M // set D to y
@R2
M=D+M // add y to product
@1
D=A
@x
M=M-D // decrement x
// end of loop body

// jump to start of loop
@WHILE
0;JMP

// end of loop
(END)
@END

0;JMP // end program