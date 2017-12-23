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
		methodInfo.displayMethodInfo(methodInfo);
		methodInfo.displayMethodInfo(new A());
		methodInfo.displayMethodInfo(new String());
	}
}