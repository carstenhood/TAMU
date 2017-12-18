// Project 7: VM Translator (part 1/2)
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "Code Writer" module described in textbook

import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

public class CodeWriter {

    private int jumpCounter;
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
            	output = pushTranslation("LCL", index, false);
            } else if (segment.equals("argument")) {
                output = pushTranslation("ARG", index, false);
            } else if (segment.equals("temp")) {
                output = pushTranslation("R5", index + 5, false);
            } else if (segment.equals("static")) {
                output = pushTranslation(String.valueOf(16 + index), index, true);
            } else if (segment.equals("pointer") && index == 0) {
                output = pushTranslation("THIS", index, true);
            } else if (segment.equals("pointer") && index == 1) {
                output = pushTranslation("THAT", index, true);
            } else if (segment.equals("this")) {
                output = pushTranslation("THIS", index, false);
            } else if (segment.equals("that")) {
                output = pushTranslation("THAT", index, false);
            }
        } else if (command.equals("C_POP")) {
            if (segment.equals("local")) {
                output = popTranslation("LCL", index, false);
            } else if (segment.equals("argument")) {
                output = popTranslation("ARG", index, false);
            } else if (segment.equals("temp")) {
                output = popTranslation("R5", index + 5, false);
            } else if (segment.equals("static")) {
                output = popTranslation(String.valueOf(16 + index), index, true);
            } else if (segment.equals("pointer") && index == 0) {
                output = popTranslation("THIS", index, true);
            } else if (segment.equals("pointer") && index == 1) {
                output = popTranslation("THAT", index, true);
            } else if (segment.equals("this")) {
                output = popTranslation("THIS", index, false);
            } else if (segment.equals("that")) {
                output = popTranslation("THAT", index, false);
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
    private String pushTranslation(String segment, int index, boolean directAddress) {

    	// read THIS or THAT (if a pointer) or data stored in the address (if static) 
    	String pointerCode = "";
    	if (!directAddress) {
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
    private String popTranslation(String segment, int index, boolean directAddress) {
        
    	// R13 either stores the address of THIS or THAT or stores the index address
    	String pointerCode = "";
    	if (directAddress) {
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