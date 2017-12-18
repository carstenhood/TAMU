// Project 6: Assembler
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "parser" module described in textbook

import java.io.FileInputStream;
import java.io.DataInputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.text.ParsePosition;
import java.text.NumberFormat;
import java.io.IOException;
import java.io.FileNotFoundException;

public class Parser {
	
	private String fileString; // convert file into string
	public String lineArray[]; // strings representing lines in file
	
	public int lineCounter;
	
	// c-tor opens the input file and gets ready to parse it 
	Parser(String inputFilename) {
		
		// initialize input stream from file
		FileInputStream fileStream = null; 
		try { 
			fileStream = new FileInputStream(inputFilename);
		} catch (FileNotFoundException exception) {
			exception.printStackTrace();
		}

		// converts input file to string
		int content;
        try {
			while ((content = fileStream.read()) != -1) {
			    fileString += (char) content; 	// convert to char and display it
			}
		} catch (IOException exception) {
			exception.printStackTrace();
		}

        DataInputStream inputStream = new DataInputStream(fileStream);
	    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
	    
	    // remove comments and trim each line
		fileString = fileString.replaceAll( "//.*|(\"(?:\\\\[^\"]|\\\\\"|.)*?\")|(?s)/\\*.*?\\*/|(?m)^[ \t]*\r?\n|null|\t", "" );
		fileString = fileString.replaceAll("(?m)^[ \t]*\r?\n", "");
		lineArray = fileString.split("\n");
	    for (int i = 0; i < lineArray.length; i++) {
	    	lineArray[i] =  lineArray[i].trim();
	    }
	    
		lineCounter = 0;
	}
	
	// helper methods to interpret and rewrite commands
	
	public char currentCommandType() {
		if (lineArray[lineCounter].startsWith("@")) {
			return 'A'; // address command
		} else if (lineArray[lineCounter].startsWith("(")) {
			return 'L'; // label
		} else {
			return 'C'; // compute command
		}
	}
	
	public boolean isNumber(String string) {
		NumberFormat formatter = NumberFormat.getInstance();
		ParsePosition position = new ParsePosition(0);
		formatter.parse(string, position);
		return (position.getIndex() == string.length());
	}

	public static String convertTo16DigitBinary(int number) {
		String binaryString = Integer.toBinaryString(number);
		StringBuilder stringBuilder = new StringBuilder();
		for (int p = 16 - binaryString.length(); p > 0; p--) {
			stringBuilder.append('0');
		}
		stringBuilder.append(binaryString);
		return stringBuilder.toString();
	}
	
	// methods to parse commands
	
	public String parseComp() {
		String line = lineArray[lineCounter]; 
		if (line.contains("=")) {
			int endIndex = line.lastIndexOf("=");
			return line.substring(endIndex + 1, line.length());
		} else if (line.contains(";")) {
			int endIndex = line.lastIndexOf(";");
			return line.substring(0, endIndex);
		} else {
			return line;
		}
	}
	
	public String parseDest() {
		String line = lineArray[lineCounter]; 
		if (line.contains("=")) {
			int endIndex = line.lastIndexOf("=");
			return line.substring(0, endIndex);
		} else {
			return null;
		}
	}
	
	public String parseJump() {
		if (lineArray[lineCounter].contains(";")) {
			String line = lineArray[lineCounter]; 
			int endIndex = line.lastIndexOf(";");
			return line.substring(endIndex + 1, line.length());
		} else {
			return null;
		}
	}
	
	public String parseSymbol() {
		String line = lineArray[lineCounter]; 
		if (line.startsWith("@")) {
			return line.replaceAll("@", "");
		} else if (line.startsWith("(")) {
			return line.replaceAll("\\((.*?)\\)", "$1");
		} else {
			return "";
		}
	}
} // end of parser class