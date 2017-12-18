// Assignment p7, CSCE-314
// Section: 502
// Student Name: CARSTEN HOOD
// UIN: 922009787

import java.util.*;

public final class Test3 {
	
	static class Food {
		@Override public String toString() {
			return "Food";
		}
	}

	static class Fruit extends Food {
		@Override public String toString() {
			return "Fruit";
		}
	}
	
	static class Apple extends Fruit {
		@Override public String toString() {
			return "Apple";
		}
	}
	
    public static void main(String[] args) {
    	System.out.println("initiating transactions...\n");

    	final Shop<Food> grocery = new Shop<Food>();

    	System.out.println("restocking 4 items individually.");
    	grocery.sell(new Apple());
    	grocery.sell(new Fruit());
    	grocery.sell(new Food());
    	grocery.sell(new Fruit());

    	System.out.println("customer buys 4 items individually:");
    	for (int i=0; i<4; i++) {
    		System.out.println(grocery.buy());
    	}
    	
    	java.util.List<Food> restock = new java.util.LinkedList<Food>();
    	restock.add(new Apple());
    	restock.add(new Fruit());
    	restock.add(new Food());
    	System.out.println("restocking with LinkedList: " + restock);
    	grocery.sell(restock);
    	
    	System.out.println("customer buys 3 items into a HashSet:");
    	final Set<Food> cart = new HashSet<Food>();
    	grocery.buy(3, cart);
    	for (Food item : cart) {
    		System.out.println(item);
    	}
    	
    	java.util.List<Food> restock2 = new java.util.ArrayList<Food>();
    	restock2.add(new Apple());
    	restock2.add(new Fruit());
    	restock2.add(new Apple());
    	restock2.add(new Fruit());
    	System.out.println("restocking with ArrayList: " + restock2);
    	grocery.sell(restock2);

    	final List<Food> basket = new ArrayList<Food>();
    	grocery.buy(3, basket);
    	System.out.println("customer buys 3 items into an ArrayList: " + basket);
    	
    	System.out.println("all transactions successful.\n");
	} 
}