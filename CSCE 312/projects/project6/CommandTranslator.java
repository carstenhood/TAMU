// Project 6: Assembler
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "code" module described in textbook

import java.util.Map;
import java.util.HashMap;

public class CommandTranslator {
	
	// declare tables
	private Map<String, String> compTable;
	private Map<String, String> destTable;
	private Map<String, String> jumpTable;
	
	public CommandTranslator() {	
		compTable = new HashMap<String, String>();
		populateCompTable();	
		
		destTable = new HashMap<String, String>();
		populateDestTable();
		
		jumpTable = new HashMap<String, String>();
		populateJumpTable();
	}

	// populate tables by mapping assembly commands to binary codes
	void populateCompTable() {
		compTable.put("0", "0101010");
		compTable.put("1", "0111111");
		compTable.put("-1","0111010");
		compTable.put("D", "0001100");
		compTable.put("A", "0110000");
		compTable.put("!D", "0001101");
		compTable.put("!A", "0110001");
		compTable.put("-D", "0001111");
		compTable.put("-A", "0110011");
		compTable.put("D+1","0011111");
		compTable.put("A+1","0110111");
		compTable.put("D+A","0000010");
		compTable.put("D-A","0010011");
		compTable.put("A-D","0000111");
		compTable.put("D&A","0000000");
		compTable.put("D|A","0010101");	
		compTable.put("D-1","0001110");
		compTable.put("A-1","0110010");
		compTable.put("M","1110000");
		compTable.put("!M","1110001");
		compTable.put("-M","1110011");
		compTable.put("M+1","1110111");
		compTable.put("M-1","1110010");
		compTable.put("D+M","1000010");
		compTable.put("D-M","1010011");
		compTable.put("M-D","1000111");
		compTable.put("D&M","1000000");
		compTable.put("D|M","1010101");
	}
	
	void populateDestTable() {
		destTable.put("NULL","000");
		destTable.put("M","001");
		destTable.put("D","010");
		destTable.put("MD","011");
		destTable.put("A","100");
		destTable.put("AM","101");
		destTable.put("AD","110");
		destTable.put("AMD","111");
	}
	
	void populateJumpTable() {
		jumpTable.put("NULL", "000");
		jumpTable.put("JGT", "001");
		jumpTable.put("JEQ", "010");
		jumpTable.put("JGE", "011");
		jumpTable.put("JLT","100");
		jumpTable.put("JNE","101");
		jumpTable.put("JLE","110");
		jumpTable.put("JMP", "111");
	}
	
	// use assembly commands to fetch corresponding binary codes
	public String getCompCode(String mn) { return compTable.get(mn); }
	public String getDestCode(String mn) { return destTable.get(mn); }
	public String getJumpCode(String mn) { return jumpTable.get(mn); }
}