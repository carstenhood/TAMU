import java.lang.Class;
import java.lang.reflect.*;

class Main3
{
	
	// main
	public static void main(String args[]){
		
		A a = new A();
		displayMethodInfo(a);	
	}
	
	// classes
	public static class A {
		
  		void foo(int T1, char T2) { }
  		int bar(int T1, long T2, float T3) { return 1; }
		static double doo(){ return 2; }
	}
	
	// functions
	static void displayMethodInfo(Object obj){
		
		// foo (A, T1, T2) -> void

		Class c = obj.getClass();
		
		Method methodsList[] = c.getDeclaredMethods();
		
		for (Method m : methodsList){
			
			if(!Modifier.isStatic(m.getModifiers())){
				
				// method's name
				System.out.print(m.getName() + "(");
				
				// method's class
				System.out.print(m.getDeclaringClass().getSimpleName().toString());
				
				// list of types for method
				Type[] typesList = m.getGenericParameterTypes();
				for (Type t : typesList){
					
					System.out.print(", " + t.toString());
				}
				
				// method's return type
				System.out.print(") -> " + m.getGenericReturnType().toString());
				System.out.println();
			}
			else{
				
				// method's name
				System.out.print(m.getName() + "(");
				
				// method's class
				//System.out.print(m.getDeclaringClass().getSimpleName().toString());
				
				// list of types for method
				Type[] typesList = m.getGenericParameterTypes();
				for (Type t : typesList){
					
					System.out.print(", " + t.toString());
				}
				
				// method's return type
				System.out.print(") -> " + m.getGenericReturnType().toString());
				System.out.println();
			}
			
			
		}
	}
}