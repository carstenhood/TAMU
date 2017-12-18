import java.lang.Class;
import java.lang.reflect.*;
import java.lang.String;

class Main4{
	
	// main
	public static void main(String args[]){
		
		if (args.length > 0) {
			
			String classSelection = args[0];
			
			System.out.println(classSelection);
			
			A a = new A();
			B b = new B();
			
			if (classSelection.equals("A")) findMethods(a);
			else if (classSelection.equals("B")) findMethods(b);
			else System.out.println("class DNE!"); System.exit(1);
		}
		else {
			
			System.out.println("error"); 
			System.exit(1);
		}		
	}
	
	// classes
	public static class A {
		
  		public static boolean fooo(int T1, char T2) { return false;}
  		public static int barr(int T1, long T2, float T3) { return 1; }
		public static int testOne() { return 1; }
		public static int testTwo(int T1, long T2, float T3) { return 1; }
		public static boolean testThree() { return false; }
		public static boolean testFour() { return true; }
		public static boolean testFive() { return false; }
	}
	
	public static class B {
		
  		public static boolean fooo(int T1, char T2) { return false;}
  		public static int barr(int T1, long T2, float T3) { return 1; }
		public static int testOne() { return 1; }
		public static int testTwo(int T1, long T2, float T3) { return 1; }
		public static boolean testThree() { return false; }
		public static boolean testFour() { return false; }
	}
	
	// functions
	static void findMethods(Object obj){
		
		Class c = obj.getClass();
		
		Method methodsList[] = c.getDeclaredMethods();
		for (Method m : methodsList){
			
			String mName = m.getName();
			String mClass = m.getDeclaringClass().toString();
			Type[] mTypesList = m.getGenericParameterTypes();
			String mReturn = m.getGenericReturnType().toString();
			
			if (mName.substring(0,4).equals("test") && mTypesList.length == 0 && mReturn.equals("boolean")){
				
				System.out.println("OK: " + mName + " succeeded");
			}
			else{
				
				System.out.println("FAILED: " + mName + " failed");
			}
		}
	}
	
	
}