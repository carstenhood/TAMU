// Assignment p6, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787
// sources:
// http://www.artima.com/lejava/articles/equality.html
// http://docs.oracle.com/javase/7/docs/api/java/util/ArrayList.html

// example command-line input:
// java Main "S" "r 0 0 1 1; c 3 3.1 0.1; l 0.0 1.1 2 5;"

import java.lang.Math;
import java.util.List;
import java.util.Random;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;

class Point {
    public double x, y;
	
    Point(double x, double y) {
		this.x = x;
		this.y = y;
	}
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof Point) {
    		Point that = (Point) obj;
    		result = (this.x == that.x) && (this.y == that.y);
    	}
    	return result;
    }
    
    public int hashcode() {
    	return 17 * (17 + (int) this.x) + (int) this.y;
    }
    
    public String toString() {
    	return String.format("(%.2f, %.2f)", x, y);
    }
}

abstract class Shape implements Comparable<Shape> {
    protected Point a; // position
    
    Point position() {
    	return a;
    }
    
    abstract double area();
    
    public int hashcode() {
    	return a.hashcode();
    }
    
    public int compareTo(Shape that) {
    	double a1 = this.area();
    	double a2 = that.area();
    	if (a1 > a2)
    		return 1;
    	else if (a1 == a2)
    		return 0;
    	else
    		return -1;
    }
    
    // helper method for converting double to string
    protected String numString(Double number) {
    	return String.format("%.2f", number);
    }
}

class Triangle extends Shape {
    private Point b;
    private Point c;
    
    Triangle(Point a, Point b, Point c) {
        this.a = a;
        this.b = b;
        this.c = c;
    }
    
    double area() {
        return 0.5 * Math.abs(a.x*b.y + b.x*c.y + c.x*a.y - a.x*c.y - c.x*b.y - b.x*a.y);
    }
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof Triangle) {
    		Triangle that = (Triangle) obj;
    		if (this.a.equals(that.a)) {

    			// calculate side lengths of this triangle
        		double x1a = this.b.x - this.a.x;
    	        double y1a = this.b.y - this.a.y;
    	        double s1a = Math.sqrt(x1a*x1a + y1a*y1a);
        		
    	        double x1b = this.c.x - this.b.x;
    	        double y1b = this.c.y - this.b.y;
    	        double s1b = Math.sqrt(x1b*x1b + y1b*y1b);
        		
    	        double x1c = this.a.x - this.c.x;
    	        double y1c = this.a.y - this.c.y;
    	        double s1c = Math.sqrt(x1c*x1c + y1c*y1c);
    	        
    			// calculate side lengths of that triangle
        		double x2a = this.b.x - this.a.x;
    	        double y2a = this.b.y - this.a.y;
    	        double s2a = Math.sqrt(x2a*x2a + y2a*y2a);
        		
    	        double x2b = this.c.x - this.b.x;
    	        double y2b = this.c.y - this.b.y;
    	        double s2b = Math.sqrt(x2b*x2b + y2b*y2b);
        		
    	        double x2c = this.a.x - this.c.x;
    	        double y2c = this.a.y - this.c.y;
    	        double s2c = Math.sqrt(x2c*x2c + y2c*y2c);
    	        
    	        // any two triangles with 3 corresponding sides are congruent
    	        if ((s1a == s2a && s1b == s2b && s1c == s2c)
    	        	|| (s1a == s2a && s1b == s2c && s1c == s2b)
    	        	|| (s1a == s2b && s1b == s2c && s1c == s2a)
    	        	|| (s1a == s2b && s1b == s2a && s1c == s2c)
    	        	|| (s1a == s2c && s1b == s2a && s1c == s2b)
    	        	|| (s1a == s2c && s1b == s2b && s1c == s2a)
    	        	) {
        			result = true;
    	        }
    		}
    	}
    	return result;
    }

    public String toString() {
        return "Triangle <" + a + ", " + b + ", " + c + ">";
    }
}

class Rectangle extends Shape {
    private Point b;
    
    Rectangle(Point a, Point b) {
        this.a = a;
        this.b = b;
    }
    
    double area() {
    	double width = Math.abs(b.x - a.x);
        double height = Math.abs(b.y - a.y);
        return width * height;
    }
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof Rectangle) {
    		Rectangle that = (Rectangle) obj;
    		
	    	double w1 = this.b.x - this.a.x;
	        double h1 = this.b.y - this.a.y;
	        double d1 = Math.sqrt(w1*w1 + h1*h1);
	        
	    	double w2 = that.b.x - that.a.x;
	        double h2 = that.b.y - that.a.y;
	        double d2 = Math.sqrt(w2*w2 + h2*h2);

    		if (this.a.equals(that.a) && d1 == d2) {
    			result = true;
    		}
    	}
    	return result;
    }
    
    public String toString() {
        return "Rectangle <" + a + ", " + b + ">";
    }
}

class Square extends Shape {
    private double l;
    
    Square(Point a, double l) {
        this.a = a;
        this.l = l;
    }
    
    double area() {
        return l * l;
    }
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof Square) {
    		Square that = (Square) obj;
    		if (this.a.equals(that.a) && this.l == that.l) {
    			result = true;
    		}
    	}
    	return result;
    }
    
    public String toString() {
        return "Square <" + a + ", " + numString(l) + ">";
    }
}

class Circle extends Shape {
    private double r;
    
    Circle(Point a, double r) {
        this.a = a;
        this.r = r;
    }
    
    double area() {
        return Math.PI * r*r;
    }
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof Circle) {
    		Circle that = (Circle) obj;
    		if (this.a.equals(that.a) && this.r == that.r) {
    			result = true;
    		}
    	}
    	return result;
    }
    
    public String toString() {
        return "Circle <" + a + ", " + numString(r) + ">";
    }
}

class LineSegment extends Shape {
    private Point b;
    
    LineSegment(Point a, Point b) {
        this.a = a;
        this.b = b;
    }
    
    double area() {
        return 0.0;
    }
    
    @Override public boolean equals(Object obj) {
    	boolean result = false;
    	if (obj instanceof LineSegment) {
    		LineSegment that = (LineSegment) obj;
    		
    		double x1 = this.b.x - this.a.x;
	        double y1 = this.b.y - this.a.y;
	        double d1 = Math.sqrt(x1*x1 + y1*y1);
	        
	    	double x2 = that.b.x - that.a.x;
	        double y2 = that.b.y - that.a.y;
	        double d2 = Math.sqrt(x2*x2 + y2*y2);

    		if (this.a.equals(that.a) && d1 == d2) {
    			result = true;
    		}
    	}
    	return result;
    }
    
    public String toString() {
        return "LineSegment <" + a + ", " + b + ">";
    }
}

class AreaCalculator {
    public static double calculate(Shape[] shapes) {
    	double area = 0.0;
    	for (int i = 0; i < shapes.length; i++) {
    		area += shapes[i].area();
    	}
    	return area;
    }
}

public class Main {
	
	// helper methods for random shape generation
	static Point randomPoint() {
		return new Point(randomNumber(), randomNumber());
	}
	
	static double randomNumber() {
	    Random random = new Random();
		double number = random.nextDouble() * 100.00;
		return number;
	}
	
	static double randomDimension() {
	    Random random = new Random();
		double number = 0.01 + random.nextDouble() * 99.99;
		return number;
	}

	// main input / output method
    public static void main(String[] args) {
      
      try {
      
    	if (args.length >= 2) {

    		Shape[] shapes = null;
    		
    		// randomly generate n shapes
    		if (args[0].equals("R")) {
    			int n = Integer.parseInt(args[1]);
    			if (n <= 0) {
					throw new IllegalArgumentException("Invalid input: please indicate more than 0 shapes.");
    			}
    			
    			shapes = new Shape[n];
    			for (int i = 0; i < n; i++) {
    				Shape s = null;
    				int random = (int) (Math.random() * 5);
    				if (random == 0) {
    					Point a = randomPoint();
    					Point b = randomPoint();
    					Point c = randomPoint();
    					
    					if (!(a.x == b.x && a.x == c.x) && !(a.y == b.y && a.y == c.y)) {
        					double slope1 = (b.y - a.y)/(b.x - a.x);
        					double slope2 = (c.y - a.y)/(c.x - a.x);
        					if (slope1 != slope2) // points are not on the same line
        						s = new Triangle(randomPoint(), randomPoint(), randomPoint());
    					}
    				} else if (random == 1) {
    					Point a = randomPoint();
    					double w = randomDimension();
    					double h = randomDimension();
    					Point b = new Point(a.x+w, a.y+h);
    					s = new Rectangle(a, b);
    				} else if (random == 2) {
    					Point a = randomPoint();
    					double l = randomDimension();
    					s = new Square(a, l);
    				} else if (random == 3) {
    					Point a = randomPoint();
    					double r = randomDimension();
    					s = new Circle(a, r);
    				} else if (random == 4) {
    					Point a = randomPoint();
    					Point b = randomPoint();
    					if (a != b)
        					s = new LineSegment(a, b);
    				}
    				
    				if (s == null)
    					i--; // retry if random generation produces invalid dimensions
    				else
        				shapes[i] = s;
    			}
    		} 
    		
    		// read in shapes
    		else if (args[0].equals("S")) {
    			String[] items = args[1].split(";");
    			shapes = new Shape[items.length];
    			for (int i = 0; i < items.length; i++) {
    				Scanner scanner = new Scanner(items[i]);
    				char shapeChar = '-';
    				if (scanner.hasNext())
    					shapeChar = scanner.next().charAt(0);
    				else
    					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
    							+ ": no character to indicate a shape.");
        			
        			// read in point and dimension values
        			List<Double> vs = new ArrayList<Double>();
        			while (scanner.hasNextDouble()) {
        				double v = scanner.nextDouble();
        				if (v >= 0.00 && v <= 100.00)
            				vs.add(v);
    					else
        					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
        							+ ": please enter values between 0.00 and 100.00.");
        			}
        			
    				if (scanner.hasNext()) // should be empty after last double is consumed
    					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
    							+ ": non-numerical characters present.");
        			        			
        			// generate shape objects
        			Shape s = null;
        			switch (shapeChar) {
        				case 't':
        					if (vs.size() == 6) {
            					Point a = new Point(vs.get(0), vs.get(1));
            					Point b = new Point(vs.get(2), vs.get(3));
            					Point c = new Point(vs.get(4), vs.get(5));
            					
            					if (a.equals(b) || a.equals(c) || b.equals(c))
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": triangle points must be unique.");
            					else if ((a.x == b.x && a.x == c.x) || (a.y == b.y && a.y == c.y))
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": triangle points cannot be on the same line.");
            					else if ((b.y - a.y)/(b.x - a.x) == (c.y - a.y)/(c.x - a.x))
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": triangle points cannot be on the same line.");
            					else
                        			s = new Triangle(a, b, c);
            				} else {
                				throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                						+ ": a triangle must have three points, i.e., 6 numbers.");
            				}
        					break;
        				case 'r':
        					if (vs.size() == 4) {
            					Point a = new Point(vs.get(0), vs.get(1));
            					Point b = new Point(vs.get(2), vs.get(3));
            					if (a.equals(b))
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": rectangle points should be unique.");
            					else if (a.x > b.x && a.y > b.y)
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": the first rectangle point should be the top-left point.");
            					else if (a.x == b.x || a.y == b.y)
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": rectangle points cannot be on the same vertical or horizontal line.");
            					else
                        			s = new Rectangle(a, b);

            				} else {
                				throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                						+ ": a rectangle must have two points, i.e., 4 numbers.");
            				}
        					break;
        				case 's':
        					if (vs.size() == 3) {
            					Point a = new Point(vs.get(0), vs.get(1));
            					double l = vs.get(2);
            					if (l != 0.0)
                        			s = new Square(a, l);
            					else 
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": a square must have a non-zero length.");
            				} else {
                				throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                						+ ": a square must have a position and a length, i.e., 3 numbers.");
            				}
        					break;
        				case 'c':
        					if (vs.size() == 3) {
            					Point a = new Point(vs.get(0), vs.get(1));
            					double r = vs.get(2);
            					if (r != 0.0)
                        			s = new Circle(a, r);
            					else 
                					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                							+ ": a circle must have a non-zero radius.");
            				} else {
                				throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                						+ ": a circle must have a position and a radius, i.e., 3 numbers.");
            				}
        					break;
        				case 'l':
        					if (vs.size() == 4) {
            					Point a = new Point(vs.get(0), vs.get(1));
            					Point b = new Point(vs.get(2), vs.get(3));
            					if (!a.equals(b))
                        			s = new LineSegment(a, b);
            					else
            						throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
            								+ ": a line segment must have two unique points.");
            				} else {
                				throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
                						+ ": a line segment must have two points, i.e., 4 numbers.");
            				}
        					break;
    					default:
        					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1)
        							+ ": no shape indicated by character '" + shapeChar + ".");
        			}
        			
        			if (s == null)
    					throw new IllegalArgumentException("Invalid input at shape entry " + (i+1) + ".");
        			else
        				shapes[i] = s;
    			}
    		} else {
				throw new IllegalArgumentException("Invalid input: indicate \"S\" or \"R\" for the first argument.");
    		}
    		
    		if (shapes == null || shapes.length < 0) {
				throw new IllegalArgumentException("Invalid input.");
    		}
    		
    		Arrays.sort(shapes);
        	String output = "Shapes ordered by area:\n";
        	for (int i = 1; i <= shapes.length; i++) {
        		if (shapes[i-1] == null)
            		output += i + ". parsing error;";
        		else
            		output += i + ". " + shapes[i-1] + ", ";
        			output += "area: " + String.format("%.2f", shapes[i-1].area()) + ";";
        		if (i != shapes.length)
        			output += "\n";
        	}
    		System.out.println(output);
    		
    		double area = AreaCalculator.calculate(shapes);
    		System.out.println("Total area: " + area + "\n");
    	
        } else {
			throw new IllegalArgumentException("Invalid input: at least two string arguments required.");
    	}

      } catch (NumberFormatException e) {
          System.err.println("Second argument '" + args[1] + "' must be an integer.");
          System.exit(1);
      } catch (IllegalArgumentException e) {
    	  System.err.println(e.getMessage());
    	  System.exit(1);
      } 

	} // end of Main method

} // end of Main class
