// Project 7: VM Translator (part 1/2)
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

        // parse input and generate output
        for (File file : inputFiles) {
            Parser parser = new Parser(file);
            while (parser.hasMoreCommands()) {
                parser.advance();
                String commandType = parser.commandType();
                if (commandType.equals("C_ARITHMETIC")) {
                    codewriter.writeArithmetic(parser.arg1());
                } else if (commandType.equals("C_POP")) {
                	codewriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                } else if (commandType.equals("C_PUSH")) {
                	codewriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                }
            }
        }

        codewriter.close();

    } // end main
    
} // end class