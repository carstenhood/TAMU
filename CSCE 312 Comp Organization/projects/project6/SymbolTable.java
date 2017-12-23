// Project 6: Assembler
// class: CSCE 312-503
// student: Carsten Hood
// UIN: 922009787

// serves as "symbol table" module described in textbook

import java.util.Map;
import java.util.HashMap;

public class SymbolTable {

	private static Map<String, String> symbolTable;
	 
	// populate table by mapping default symbols to memory locations
	public SymbolTable() {	
		symbolTable = new HashMap<String, String>();
		populateSymbolTable();
	 }
	
	void populateSymbolTable() {
		symbolTable.put("R0", "0");
		symbolTable.put("R1", "1");
		symbolTable.put("R2", "2");
		symbolTable.put("R3", "3");
		symbolTable.put("R4", "4");
		symbolTable.put("R5", "5");
		symbolTable.put("R6", "6");
		symbolTable.put("R7", "7");
		symbolTable.put("R8", "8");
		symbolTable.put("R9", "9");
		symbolTable.put("R10", "10");
		symbolTable.put("R11", "11");
		symbolTable.put("R12", "12");
		symbolTable.put("R13", "13");
		symbolTable.put("R14", "14");
		symbolTable.put("R15", "15");

		symbolTable.put("SP", "0");
		symbolTable.put("LCL", "1");
		symbolTable.put("ARG", "2");
		symbolTable.put("THIS", "3");
		symbolTable.put("THAT", "4");
		
		symbolTable.put("SCREEN", "16384");
		symbolTable.put("KBD", "24576");
	}
	
	public String getAddress(String symbol) {
		return symbolTable.get(symbol);	
	}
	
	public boolean containsSymbol(String symbol){
		return symbolTable.containsKey(symbol);
	}
	
	// allow for adding more symbols
	public void addSymbol(String symbol, String location) {
		symbolTable.put(symbol, location);	
	}
}
