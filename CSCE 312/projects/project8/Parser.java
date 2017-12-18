// Project 8: VM Translator (part 2/2)
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "Parser" module described in textbook

import java.io.File;
import java.util.Scanner;
import java.io.FileNotFoundException;
import java.util.IllegalFormatException;

public class Parser {
	
    private Scanner scanner;
    
    private String commandType;    
    private String arg1;
    private int arg2;

    // opens input file and prepares for parsing
	// input: .vm file; output: _;
    public Parser(File inputFile) {
    	try {
            scanner = new Scanner(inputFile);
            
            // trim comments & white space from scanner
            String trimmedFileString = "";
            String line = "";
            while (scanner.hasNext()) {                
                line = scanner.nextLine();
                int index = line.indexOf("//");
                if (index != -1) {
                    line = line.substring(0, index);
                }
                line.trim();
                if (line.length() > 0) {
                	trimmedFileString += line + "\n";
                }
            }
            scanner = new Scanner(trimmedFileString.trim());
        
        } catch (FileNotFoundException exception) {
            System.out.println("parser could not find file");
        }
        
    	// initialize variables
        commandType = "";
        arg1 = "";
        arg2 = 0;
    }

    // checks that input file has more to parse
	// input: _; output: more-commands boolean;
    public boolean hasMoreCommands() {
       return scanner.hasNextLine();
    }
    
    // returns first argument of command
	// input: _; output: String argument;
    public String arg1() {
    	return arg1;
    }

    // returns second argument of command
	// input: _; output: integer argument;
    public int arg2() {
    	return arg2;
    }

    // determine next command's arguments and type
	// input: _; output: _;
    public void advance() {
        String[] args = scanner.nextLine().split(" ");
        if (args[0].equals("return")) {
        	commandType = "C_RETURN";
            arg1 = args[0]; // set to return command
        } else if (isCommandArithmetic(args[0])) {
        	commandType = "C_ARITHMETIC";
            arg1 = args[0]; // set to arithmetic command
        } else if (setOtherCommandType(args[0])) {
            arg1 = args[1]; // set to memory segment
            if (commandUsesArg2()) {
                arg2 = Integer.parseInt(args[2].trim()); // set to int
            }
        }
    }
    
    // helper function to determine arithmetic commands
	// input: command; output: is-arithmetic boolean;
    public boolean isCommandArithmetic(String command) {
    	return command.equals("add")
    			|| command.equals("sub")
        		|| command.equals("neg")
        		|| command.equals("and")
        		|| command.equals("or")
        		|| command.equals("not")
        		|| command.equals("eq")
        		|| command.equals("gt")
        		|| command.equals("lt");
    }
    
    // helper function to determine command type
	// input: command; output: is-other-command boolean;
    public boolean setOtherCommandType(String command) {
    	if (command.equals("push")) {
        	commandType = "C_PUSH";
        } else if (command.equals("pop")) {
            commandType = "C_POP";
        } else if (command.equals("label")) {
            commandType = "C_LABEL";
        } else if (command.equals("goto")) {
            commandType = "C_GOTO";
        } else if (command.equals("if-goto")) {
            commandType = "C_IF";
        } else if (command.equals("call")) {
            commandType = "C_CALL";
        } else if (command.equals("function")) {
            commandType = "C_FUNCTION";
        } else {
        	return false;
        }
    	return true;
    }

    // helper function to determine if current command is push, pop, function, or call
	// input: (current command type); output: is-relevant-command boolean;
    public boolean commandUsesArg2() {
    	return commandType.equals("C_PUSH")
        		|| commandType.equals("C_POP")
        		|| commandType.equals("C_FUNCTION")
        		|| commandType.equals("C_CALL");
    }

    // returns current command type
	// input: _; output: current command type;
    public String commandType() {
        return commandType;
    }

} // end Parser