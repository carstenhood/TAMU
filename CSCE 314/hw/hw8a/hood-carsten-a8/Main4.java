// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.*;
import java.lang.Class;
import java.lang.reflect.*;

public class Main4 {
	private static boolean isTestMethod(Method m) {
		return (m.getName().length() >= "test".length())
    			&& (m.getName().substring(0, 4).equals("test"))
    			&& (m.getParameterCount() == 0)
    			&& (m.getReturnType() == java.lang.Boolean.TYPE)
				&& (Modifier.isStatic(m.getModifiers()))
				&& (Modifier.isPublic(m.getModifiers()));
	}
	
	public static void main (String[] args) {
    	if (args.length >= 1) {
    		try {
        		String className = args[0];
        		Class c = Class.forName(className);
        		Object o = c.newInstance();
        		Method methods[] = c.getDeclaredMethods();
                for (Method m : methods) {
                	if (isTestMethod(m)) {
                		if ((boolean) m.invoke(o))
                    		System.out.println("OK: " + m.getName() + " succeeded");
                    	else
                    		System.out.println("FAILED: " + m.getName() + " failed");
                	}	
                }
    		} catch (Exception e) {
    			System.out.println("Exception thrown. Check class name. Operation terminated.");
    		}
    	}
	}
}