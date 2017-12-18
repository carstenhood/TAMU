// Assignment p6, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

// sources:
// 1. http://www.artima.com/lejava/articles/equality.html
// 2. http://docs.oracle.com/javase/7/docs/api/java/util/ArrayList.html
// 3. http://docs.oracle.com/javase/tutorial/displayCode.html?code=http://docs.oracle.com/javase/tutorial/uiswing/examples/start/HelloWorldSwingProject/src/start/HelloWorldSwing.java
// 4. http://stackoverflow.com/questions/9713432/drawing-simple-rectangles-on-a-jframe-in-java

// example command-line input:
// java Main "S" "r 0 0 1 1; c 3 3.1 0.1; l 0.0 1.1 2 5;"

import java.lang.Math;
import java.util.Random;
import java.util.Scanner;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

import javax.swing.*;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Color;

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
    		
    		// output shapes
    		Arrays.sort(shapes);
        	String output = "Shapes ordered by area:\n";
        	for (int i = 1; i <= shapes.length; i++) {
        		output += i + ". " + shapes[i-1] + ", ";
        		output += "area: " + String.format("%.2f", shapes[i-1].area()) + ";";
        		if (i != shapes.length)
        			output += "\n";
        	}
    		System.out.println(output);
    		
    		// output total area
    		double area = AreaCalculator.calculate(shapes);
    		System.out.println("Total area: " + area + "\n");
    		
    		// draw shapes
    		System.out.println("Drawing shapes...");
            JFrame frame = new JFrame("Shapes");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(1000,1000);
            GraphicShapes graphicShapes = new GraphicShapes();
            graphicShapes.graphicShapes = shapes;
            frame.getContentPane().add(graphicShapes);
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
    		
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

class GraphicShapes extends JPanel {
	public Shape[] graphicShapes;

	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		for (int i = 0; i < graphicShapes.length; i++) {
			graphicShapes[i].draw(g);
		}
	}
}


