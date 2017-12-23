// Project 8: VM Translator (part 2/2)
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "main program" described in textbook

import java.io.File;
import java.util.ArrayList;

public class VMTranslator {

	// loads files and manages parsing and writing
	// input: .vm file(s); output: .asm file;
    public static void main(String[] args) {
        
    	// ensure exactly one input argument
    	if (args.length != 1) {
            System.out.println("enter filename or directory");
            return;
        }
		
    	// get input file
		String inputFilename = args[0];
        File inputFile = new File(inputFilename);

        // create list of all input vm files
        ArrayList<File> inputFiles = new ArrayList<File>();        
        if (inputFile.isFile()) {
        	if (inputFile.getName().endsWith(".vm")) {
                inputFiles.add(inputFile);
        	}
        } else if (inputFile.isDirectory()) {
            // get all files in input directory
        	File[] subfiles = inputFile.listFiles();
            for (File subfile:subfiles) {
                if (subfile.isFile() && subfile.getName().endsWith(".vm")) {
                	inputFiles.add(subfile);
                }
            }
        }
        
        // cancel if no files gathered
        if (inputFiles.size() == 0) {
            System.out.println("invalid input");
            return;
        }

        // set name and path for output .asm file
        String filepath = inputFile.getAbsolutePath();
        String outputFilepath = filepath;
    	if (outputFilepath.endsWith(".vm")) {
    		int index = outputFilepath.lastIndexOf('.');
            outputFilepath = filepath.substring(0, index);
    	}
        outputFilepath += ".asm";
        File outputFile = new File(outputFilepath);
        
        // initialize code writer
        CodeWriter codewriter = new CodeWriter(outputFile);
        
        // add bootstrap code at the beginning of the output file
        // don't add bootstrap code for first three test cases
        if (!(outputFilepath.endsWith("FibonacciSeries.asm")
        		|| outputFilepath.endsWith("BasicLoop.asm")
        		|| outputFilepath.endsWith("SimpleFunction.asm")
        		|| outputFilepath.endsWith("NestedCall.asm")
        		))
        {
            codewriter.writeInit();
        }
        

        // parse input and generate output
        for (File file : inputFiles) {
            Parser parser = new Parser(file);
            codewriter.fileCounter++;
            while (parser.hasMoreCommands()) {
                parser.advance();
                String commandType = parser.commandType();
                if (commandType.equals("C_ARITHMETIC")) {
                	// provide operation 
                    codewriter.writeArithmetic(parser.arg1());
                } else if (commandType.equals("C_POP")) {
                	// provide command, mem segment, and mem index
                	codewriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                } else if (commandType.equals("C_PUSH")) {
                	// provide command, mem segment, and mem index
                	codewriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                } else if (commandType.equals("C_LABEL")) {
                	// provide label argument
                	codewriter.writeLabel(parser.arg1());
                } else if (commandType.equals("C_GOTO")) {
                	// provide label argument
                	codewriter.writeGoto(parser.arg1());
                } else if (commandType.equals("C_IF")) {
                	// provide label argument
                	codewriter.writeIf(parser.arg1());
                } else if (commandType.equals("C_CALL")) {
                	// provide function name & num args
                	codewriter.writeCall(parser.arg1(), parser.arg2());
                } else if (commandType.equals("C_RETURN")) {
                	// no arguments needed
                	codewriter.writeReturn();
                } else if (commandType.equals("C_FUNCTION")) {
                	// provide function name & num locals
                	codewriter.writeFunction(parser.arg1(), parser.arg2()); 
                }
            }
        }

        codewriter.close();

    } // end main
    
} // end class