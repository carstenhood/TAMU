// Project 6: Assembler
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "main program" module described in textbook

import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;

public class Assembler {
	
	public static int symbolCounter = 0;
	public static int RAMcounter = 16;
	
	public static void main(String[] args) {
		
		// initialize symbol and code tables
		CommandTranslator commandTranslator = new CommandTranslator();
		SymbolTable symbolTable = new SymbolTable();
		
		// generate output .hack file with input filename
		String inputFilename = args[0];
		String filename = inputFilename.substring(0, args[0].indexOf('.'));
		File outputFile = new File(filename + ".hack");
		
		// create writer for output file
		FileWriter fileWriter = null;
		try {
			fileWriter = new FileWriter(outputFile.getAbsoluteFile());
		} catch (IOException exception) {
			exception.printStackTrace();
		}
		BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
		
		// initialize parser using parser module
		Parser parser = new Parser(args[0]);
		
		// first pass through file adds labels to symbol table 
		while (parser.lineCounter < parser.lineArray.length) {  
			if (parser.currentCommandType() == 'L') {
				symbolTable.addSymbol(parser.parseSymbol(), Integer.toString(symbolCounter));
			} else {
				symbolCounter++;
			}
			parser.lineCounter++;
		} // end first pass
		
		parser.lineCounter = 0; // reset counter for second pass
		
		// second pass through file generates binary code
		while (parser.lineCounter < parser.lineArray.length) {
			
			// handle A-type address commands
			if (parser.currentCommandType() == 'A') {
				if (parser.lineArray[parser.lineCounter].startsWith("@")) {
					String address = parser.parseSymbol();
					boolean write = false; 
					if (parser.isNumber(address)) {
						write = true;
					} else { // if not a number, turn to symbol table
						if (!symbolTable.containsSymbol(address)) { // create new symbol if not in table
							symbolTable.addSymbol(address, Integer.toString(RAMcounter));
							RAMcounter++;
						} 
						if (symbolTable.containsSymbol(address)) { // use symbol if in table
							address = symbolTable.getAddress(address);
							write = true;
						}
					}
					if (write) {
						int addressInt = Integer.parseInt(address);
						address = parser.convertTo16DigitBinary(addressInt);
						try {
							bufferedWriter.write(address + '\n');
						} catch (IOException exception) {
							exception.printStackTrace();
						}	
					}
				}
			} // end A-type command translation
			
			// handle C-type computation commands
			else if (parser.currentCommandType() == 'C') {
				
				// initialize code components
				String nullCommand = "NULL";
				String compCode = "";
				String destCode = "";
				String jumpCode = "";
				String fillerCode = "111";
				
				// convert assembly commands to binary code
				boolean write = false;
				if (parser.lineArray[parser.lineCounter].contains("=")) {					
					compCode = commandTranslator.getCompCode(parser.parseComp());
					destCode = commandTranslator.getDestCode(parser.parseDest());
					jumpCode = commandTranslator.getJumpCode(nullCommand);
					write = true;
				} else if (parser.lineArray[parser.lineCounter].contains(";")) {
					compCode = commandTranslator.getCompCode(parser.parseComp());
					destCode = commandTranslator.getDestCode(nullCommand);
					jumpCode = commandTranslator.getJumpCode(parser.parseJump());
					write = true;
				}
				
				// write complete binary code to file
				if (write) {
					try {
						bufferedWriter.write(fillerCode + compCode + destCode + jumpCode + '\n');
					} catch (IOException exception) {
						exception.printStackTrace();
					}
				}
				
			} // end C-type command translation
			
			parser.lineCounter++;
		
		} // end second pass
		
		try {
			bufferedWriter.close();
		} catch (IOException exception) {
			exception.printStackTrace();
		}

	} // end of main method

} // end of Assembler class