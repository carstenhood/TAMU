
// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.lang.Class;
import java.lang.reflect.*;

class MethodInfo {
	static void displayMethodInfo(Object obj) {
	    Class c = obj.getClass();
        Method methods[] = c.getDeclaredMethods();
        String output = "";
        for (Method method : methods) {
        	output += method.getName() + " (";
        	if (!Modifier.isStatic(method.getModifiers()))
        		output += c.getSimpleName() + ", ";
        	for (Type type : method.getParameterTypes()) {
        		output += type.getTypeName() + ", ";
        	}
        	if (output.charAt(output.length()-1) == ' ')
        		output = output.substring(0, output.length()-2);
        	output += ") -> " + method.getReturnType().getSimpleName() + "\n";
        }
        
        System.out.println(output);
	}
}