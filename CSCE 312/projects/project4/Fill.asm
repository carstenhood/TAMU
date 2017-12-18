// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

@SCREEN
D=A	
@current_position
M=D // set current position to screen
@8191 // set screen size
D=A
@SCREEN
D=D+A
@max_position
M=D	// set D to max position
	
(START)
@KBD
D=M // set D to keyboard value
@WHITE
D;JEQ // jump to WHITE if key is pressed
@BLACK
0;JMP // jump to BLACK if no key is pressed

(BLACK)
@current_position
A=M
M=-1 // set the current position to -1
@INCR
0;JMP // jump to increment

(WHITE)
@current_position
A=M
M=0
@INCR // jump to increment

(INCR)
@current_position
D=M+1
M=D // reset screen position if > max_position

@max_position
D=D-M // set D to current_position minus max_position
D
@START
D;JNE // don't reset if next position is already same color
	
@SCREEN
D=A
@current_position
M=D // reset
@START
0;JMP