
// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

class MyClass {
	public void testVoid() { } // ignored; wrong return type.
	public static boolean test() { // should pass.
		return true; 
	}
	private static boolean testPrivate() { // ignored; isn't public.
		return false; 
	}
	public static boolean testTrue() { // should pass.
		return true; 
	}
	public static boolean testFalse() { // should fail.
		return false; 
	}
	public static int testInt() { // ignored; wrong return type.
		return -1; 
	}
	public static boolean notTest() { // ignored; name doesn't begin w test.
		return false;
	}
	public boolean testNotStatic() { // ignored; isn't static.
		return true; 
	}
	protected static boolean testProtected() { // ignored; isn't public.
		return false; 
	}
	static boolean test2() { // ignored; isn't public.
		return false; 
	}
}