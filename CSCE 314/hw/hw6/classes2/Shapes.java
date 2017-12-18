// Assignment p6, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import javax.swing.*;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Color;

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
    abstract void draw(Graphics g);
    
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
    
    public void draw(Graphics g) {
        int[] xPoints = {(int) a.x, (int) b.x, (int) c.x};
        int[] yPoints = {(int) a.y, (int) b.y, (int) c.y};
        Graphics2D g2 = (Graphics2D) g;
        g2.drawPolyline(xPoints, yPoints, 3);
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
    
    public void draw(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;
        g2.drawRect((int) a.x, (int) a.y, (int) (b.x-a.x), (int) (b.y-a.y));
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
    
    public void draw(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;
        g2.drawRect((int) a.x, (int) a.y, (int) l, (int) l);
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
    
    public void draw(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;
        g2.drawOval((int) (a.x - r), (int) (a.y - r), (int) (2*r), (int) (2*r));
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
    
    public void draw(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;
        g2.drawLine((int) a.x, (int) a.y, (int) b.x, (int) b.y);
    }
    
    public String toString() {
        return "LineSegment <" + a + ", " + b + ">";
    }
}

