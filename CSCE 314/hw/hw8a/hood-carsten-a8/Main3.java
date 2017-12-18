// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.*;

// test class
class A {
	  void foo (boolean b, String s) {
		  if (b) System.out.println(s);
	  }
	  
	  <T> int bar (T a, int b, double c) {
		  return 0; 
	  }
	  
	  static double doo() {
		  return 0.00;
	  }
}

public class Main3 {
	public static void main (String[] args) {
		MethodInfo methodInfo = new MethodInfo();
		
		System.out.println("Starting tests...\n");

		System.out.println("-------------\nTesting String class:\n");
		methodInfo.displayMethodInfo(new String());
		
		System.out.println("-------------\nTesting MethodInfo reflectively:\n");
		methodInfo.displayMethodInfo(methodInfo);
		
		System.out.println("-------------\nTesting test class A:\n");
		methodInfo.displayMethodInfo(new A());
		
		System.out.println("Tests complete.\n");
	}
}