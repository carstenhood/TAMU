// Project 8: VM Translator (part 2/2)
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "Code Writer" module described in textbook

import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

public class CodeWriter {

    private static int jumpCounter, labelCounter;
    public static int fileCounter;
    private BufferedWriter bw;
    
    // prepares to write to output file
	// input: output .asm file; output: _;
    public CodeWriter(File outputFile) {
    	// create writer for output file
		FileWriter fileWriter = null;
		try {
			fileWriter = new FileWriter(outputFile.getAbsoluteFile());
		} catch (IOException exception) {
			exception.printStackTrace();
		}

		bw = new BufferedWriter(fileWriter);
		
		jumpCounter = -1;
		labelCounter = -1;
		fileCounter = 0;
    }
    
    // ----------------------------
    
    // translates push & pop commands to assembly code and writes to output file
	// input: push or pop command, mem segment, index; output: (writes to file);
    public void writePushPop(String command, String segment, int index) {
    	
    	String output = "";
        if (command.equals("C_PUSH")) {
            if (segment.equals("constant")) {
                output = "@" + index + "\n"
                		+ "D=A\n"
                		+ "@SP\n"
                		+ "A=M\n"
                		+ "M=D\n"
                		+ "@SP\n"
                		+ "M=M+1\n";
            } else if (segment.equals("local")) {
            	output = pushTranslation("LCL", index);
            } else if (segment.equals("argument")) {
                output = pushTranslation("ARG", index);
            } else if (segment.equals("temp")) {
                output = pushTranslation("R5", index + 5);
            } else if (segment.equals("static")) {
                output = pushTranslation("file" + fileCounter + index, -1);
            } else if (segment.equals("pointer") && index == 0) {
                output = pushTranslation("THIS", -1);
            } else if (segment.equals("pointer") && index == 1) {
                output = pushTranslation("THAT", -1);
            } else if (segment.equals("this")) {
                output = pushTranslation("THIS", index);
            } else if (segment.equals("that")) {
                output = pushTranslation("THAT", index);
            }
        } else if (command.equals("C_POP")) {
            if (segment.equals("local")) {
                output = popTranslation("LCL", index);
            } else if (segment.equals("argument")) {
                output = popTranslation("ARG", index);
            } else if (segment.equals("temp")) {
                output = popTranslation("R5", index + 5);
            } else if (segment.equals("static")) {
                output = popTranslation("file" + fileCounter + index, -1);
            } else if (segment.equals("pointer") && index == 0) {
                output = popTranslation("THIS", -1);
            } else if (segment.equals("pointer") && index == 1) {
                output = popTranslation("THAT", -1);
            } else if (segment.equals("this")) {
                output = popTranslation("THIS", index);
            } else if (segment.equals("that")) {
                output = popTranslation("THAT", index);
            }
        }
        
        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }

    // translates push commands 
	// input: mem segment, mem index, is-direct-address; output: assembly commands;
    private String pushTranslation(String segment, int index) {
    	
    	// read THIS or THAT (if a pointer) or data stored in the address (if static) 
    	String pointerCode = "";
    	if (index != -1) {
    		pointerCode = "@" + index + "\n"
    				+ "A=D+A\n"
    				+ "D=M\n";
    	}

        return "@" + segment + "\n"
        		+ "D=M\n"
                + pointerCode
                + "@SP\n"
                + "A=M\n"
                + "M=D\n"
                + "@SP\n"
                + "M=M+1\n";
    }

    // translates pop commands 
	// input: mem segment, mem index, is-direct-address; output: assembly commands;
    private String popTranslation(String segment, int index) {
        
    	// R13 either stores the address of THIS or THAT or stores the index address
    	String pointerCode = "";
    	if (index == -1) {
    		pointerCode = "D=A\n";
    	} else {
    		pointerCode = "D=M\n"
					+ "@" + index + "\n"
					+ "D=D+A\n";
    	}

        return "@" + segment + "\n"
                + pointerCode
                + "@R13\n"
                + "M=D\n"
                + "@SP\n"
                + "AM=M-1\n"
                + "D=M\n"
                + "@R13\n"
                + "A=M\n"
                + "M=D\n";
    }

    // ----------------------------

    // translates arithmetic to assembly code and writes to output file
	// input: command; output: (writes to file);
    public void writeArithmetic(String command) {

    	String output = "";
    	if (command.equals("neg")) {
            output = "D=0\n"
            		+ "@SP\n"
            		+ "A=M-1\n"
            		+ "M=D-M\n";
    	} else if (command.equals("not")) {
            output = "@SP\n"
            		+ "A=M-1\n"
            		+ "M=!M\n";
    	}
    	
    	else if (command.equals("add")) {
            output = arithmeticTranslation("M=M+D\n");
        } else if (command.equals("sub")) {
            output = arithmeticTranslation("M=M-D\n");
        } else if (command.equals("and")) {
            output = arithmeticTranslation("M=M&D\n");
        } else if (command.equals("or")) {
            output = arithmeticTranslation("M=M|D\n");
        }
    	
        else if (command.equals("eq")) {
            output = jumpTranslation("JNE");
        } else if (command.equals("gt")) {
            output = jumpTranslation("JLE");
        } else if (command.equals("lt")) {
            output = jumpTranslation("JGE");
        } 

        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }

    // translates add, sub, & or operations
	// input: operation; output: assembly commands;
    private String arithmeticTranslation(String operation) {
        return "@SP\n"
                + "AM=M-1\n"
                + "D=M\n"
                + "A=A-1\n"
                + operation;
    }

    // translates gt, lt, & eq operations
	// input: assembly jump command; output: assembly commands;
    private String jumpTranslation(String jumpType) {
    	jumpCounter++;
        return	"@SP\n"
                + "AM=M-1\n"
                + "D=M\n"
                + "A=A-1\n"
                + "D=M-D\n"
                + "@ELSE" + jumpCounter + "\n"
                + "D;" + jumpType + "\n"
                + "@SP\n"
                + "A=M-1\n"
                + "M=-1\n"
                + "@IF" + jumpCounter + "\n"
                + "0;JMP\n"
                + "(ELSE" + jumpCounter + ")\n"
                + "@SP\n"
                + "A=M-1\n"
                + "M=0\n"
                + "(IF" + jumpCounter + ")\n";
    }
    
    // ----------------------------
    
    // translates return command to assembly code and writes to output file
	// input: _; output: (writes to file);
    public void writeInit() {
    	String output = "@256\n"
                + "D=A\n"
                + "@SP\n"
                + "M=D\n";
    	
        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
        
        writeCall("Sys.init", 0);
    }
    
    // translates label command to assembly code and writes to output file
	// input: label (string); output: (writes to file);
    public void writeLabel(String label) {
    	String output = "(" + label + ")\n";

        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }
    
    // translates goto command to assembly code and writes to output file
	// input: label (string); output: (writes to file);
    public void writeGoto(String label) {
    	String output = "@" + label + "\n"
    			+ "0;JMP\n";
    	
        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }
    
    // translates if-goto command to assembly code and writes to output file
	// input: label (string); output: (writes to file);
    public void writeIf(String label) {
    	String output = "@SP\n"
			        + "AM=M-1\n"
			        + "D=M\n"
	    			+ "@" + label + "\n"
	    			+ "D;JNE\n";
    	
        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }
    
    // translates function call to assembly code and writes to output file
	// input: functionName (string), numArgs (int); output: (writes to file);
    public void writeCall(String functionName, int numArgs) {

    	// define return address
    	labelCounter++;
    	String returnLabel = "RETURN_FROM_" + functionName + "_" + labelCounter;

    	// push return address to stack
    	String output = "@" + returnLabel + "\n"
    			+ "D=A\n"
    			+ "@SP\n"
    			+ "A=M\n"
    			+ "M=D\n"
    			+ "@SP\n"
    			+ "M=M+1\n";

    	// push frame segments
    	String segments[] = {"LCL", "ARG", "THIS", "THAT"};
    	for (String segment : segments) {
    		output += pushTranslation(segment, -1);
    	}
    	
    	// prepares memory
    	output += "@SP\n"
                + "D=M\n"
                + "@" + (numArgs + 5) + "\n"
                + "D=D-A\n" // start at SP - (5 + numArgs)
                + "@ARG\n" // set ARG to first of args
                + "M=D\n"
                + "@SP\n"
                + "D=M\n"
                + "@LCL\n"
                + "M=D\n" // set LCL to SP
                + "@" + functionName + "\n" // jump to function
                + "0;JMP\n"
                + "(" + returnLabel + ")\n"; // mark return label
    	
        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }
    
    // translates return command to assembly code and writes to output file
	// input: _; output: (writes to file);
    public void writeReturn() {
    	
    	String output = "@LCL\n"
                + "D=M\n"
                + "@R11\n"
                + "M=D\n" // store LCL pointer at R11
                + "@5\n"
                + "A=D-A\n"
                + "D=M\n"
                + "@R12\n"
                + "M=D\n" // store LCL - 5 (return) pointer at R12
                + popTranslation("ARG", 0)
                + "@ARG\n"
                + "D=M\n"
                + "@SP\n"
                + "M=D+1\n"; // SP -> ARG pointer + 1
                
    	// reset segments
    	String segments[] = {"THAT", "THIS", "ARG", "LCL"};
    	for (String segment : segments) {
    		output += "@R11\n"
    				+ "D=M-1\n"
                    + "AM=D\n" // decrement R11 pointer
                    + "D=M\n"
                    + "@" + segment + "\n"
                    + "M=D\n"; // set segment to R11 content
    	}

    	// jump to return address
        output += "@R12\n"
                + "A=M\n"
                + "0;JMP\n";

        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }

    // translates function declaration to assembly code and writes to output file
	// input: functionName (string), numLocals (int); output: (writes to file);
    public void writeFunction(String functionName, int numLocals) {
    	
    	// declare label for function entry
    	String output = "(" + functionName + ")\n";

        // write assembly code to file
        try {
			bw.write(output);
		} catch (IOException exception) {
			exception.printStackTrace();
		}
        
        // init all local variables to 0
        for (int k = 0; k < numLocals; k++) {
            writePushPop("C_PUSH", "constant", 0);
        }
    }
    
    // ----------------------------

    // close writer to output file
	// input: _; output: _;
    public void close() {
		try {
			bw.close();
		} catch (IOException exception) {
			exception.printStackTrace();
		}
    }

} // end CodeWriter