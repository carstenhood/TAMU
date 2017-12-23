// Assignment p6, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

class AreaCalculator {
    public static double calculate(Shape[] shapes) {
    	double area = 0.0;
    	for (int i = 0; i < shapes.length; i++) {
    		area += shapes[i].area();
    	}
    	return area;
    }
}
